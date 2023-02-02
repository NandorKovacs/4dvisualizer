#include "renderer.h"

#include <GL/glew.h>

#define GLT_IMPLEMENTATION
#include <cmath>
#include <cmrc/cmrc.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>
#include <iostream>
#include <sstream>

#include "../../geometry/intersect.h"
#include "../../lib/camera_manager.h"
#include "../../lib/errors.h"
#include "../../lib/shader_loader.h"
#include "../../third_party/glText-master/gltext.h"

CMRC_DECLARE(viz_multicube_glsl);

namespace viz {

Renderer::Renderer(glm::ivec2 window_size, CameraManager& camera_manager,
                   HyperplaneManager& hyperplane_manager, World const& world)
    : camera_manager{camera_manager},
      window_size{window_size},
      hyperplane_manager{hyperplane_manager},
      world{world},
      text1{gltCreateText()} {
  if (!gltInit()) {
    throw std::runtime_error{"Could not initialize glt"};
  }

  cmrc::embedded_filesystem fs = cmrc::viz_multicube_glsl::get_filesystem();

  cmrc::file vshader = fs.open("light_vshader.glsl");
  cmrc::file fshader = fs.open("light_fshader.glsl");
  light_prog =
      create_shader_program(std::string{vshader.begin(), vshader.end()},
                            std::string{fshader.begin(), fshader.end()});

  cmrc::file color_vshader = fs.open("color_vshader.glsl");
  cmrc::file color_fshader = fs.open("color_fshader.glsl");
  color_prog = create_shader_program(
      std::string{color_vshader.begin(), color_vshader.end()},
      std::string{color_fshader.begin(), color_fshader.end()});

  light_loc.mv = glGetUniformLocation(light_prog, "mv_matrix");
  CHECK_GL();
  light_loc.proj = glGetUniformLocation(light_prog, "proj_matrix");
  CHECK_GL();

  light_loc.mv = glGetUniformLocation(color_prog, "mvc_matrix");
  CHECK_GL();
  light_loc.proj = glGetUniformLocation(color_prog, "projc_matrix");
  CHECK_GL();

  light_loc.material_mat = glGetUniformLocation(light_prog, "material.mat");
  CHECK_GL();
  light_loc.material_shine = glGetUniformLocation(light_prog, "material.shine");
  CHECK_GL();

  light_loc.directional_mat =
      glGetUniformLocation(light_prog, "directional_light.mat");
  CHECK_GL();
  light_loc.directional_direction =
      glGetUniformLocation(light_prog, "directional_light.direction");
  CHECK_GL();

  light_loc.global_ambient = glGetUniformLocation(light_prog, "global_ambient");
  CHECK_GL();

  light_loc.normal_matrix_transform =
      glGetUniformLocation(light_prog, "normal_matrix_transform");
  CHECK_GL();

  cmrc::file wireframe_vshader = fs.open("wireframe_vshader.glsl");
  cmrc::file wireframe_fshader = fs.open("wireframe_fshader.glsl");
  wireframe_prog = create_shader_program(
      std::string{wireframe_vshader.begin(), wireframe_vshader.end()},
      std::string{wireframe_fshader.begin(), wireframe_fshader.end()});

  wireframe_loc.mv = glGetUniformLocation(wireframe_prog, "mv_matrix");
  CHECK_GL();
  wireframe_loc.proj = glGetUniformLocation(wireframe_prog, "proj_matrix");
  CHECK_GL();

  glGenVertexArrays(1, vao);
  CHECK_GL();

  glGenBuffers(NUM_VBOs, vbo);
  CHECK_GL();
}

Renderer::~Renderer() { gltDeleteText(text1); }

void Renderer::setup_basic_projection(GLuint const mv_loc,
                                      GLuint const proj_loc) {
  aspect = (float)window_size.x / (float)window_size.y;
  pMat = glm::perspective(1.04721f, aspect, 0.1f, 1000.0f);

  glm::mat4& vMat = camera_manager.get_transform();

  mMat = glm::rotate(glm::mat4(1.0), 60.0f, glm::vec3(0, 1, 0));
  mMat = glm::translate(glm::mat4(1.0f), cube_loc) * mMat;

  mvMat = vMat * mMat;

  normal_matrix_transform = glm::transpose(glm::inverse(mvMat));

  glm::vec4& origin = hyperplane_manager.get_origin();

  glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mvMat));
  CHECK_GL();
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(pMat));
  CHECK_GL();
}

namespace {
void push_pt(std::vector<float>& f, glm::vec3 const& pt) {
  f.push_back(pt.x);
  f.push_back(pt.y);
  f.push_back(pt.z);
  // f.push_back(color);
};
void push_pt(std::vector<float>& f, glm::vec3 const& pt, int color) {
  f.push_back(pt.x);
  f.push_back(pt.y);
  f.push_back(pt.z);
  f.push_back(color);
};
}  // namespace

void Renderer::setup_vertices() {
  triangle_vertices.clear();
  triangle_normals.clear();
  v_lines.clear();
  int count = 0;

  for (Cube const& cube : world.cubes) {
    Hyperplane transformed_hyperplane =
        hyperplane_manager.get_hyperplane(cube.transformation);
    Hyperplane const& hyperplane = hyperplane_manager.get_hyperplane();

    auto convert_pt = [&](glm::vec3 pt) {
      return hyperplane.to_rel(
          cube.transformation.forward(transformed_hyperplane.to_abs(pt)));
    };

    auto handle_triangle = [&](intersect::Triangle const& tt,
                               glm::vec3 const& nn, int color) {
      glm::vec3 normal = hyperplane.to_rel(
          hyperplane.pos -
          cube.transformation.forward_rotate(transformed_hyperplane.to_abs(nn) -
                                             transformed_hyperplane.pos));

      intersect::Triangle t{{convert_pt(tt.pts[0]), convert_pt(tt.pts[1]),
                             convert_pt(tt.pts[2])}};

      for (int i = 0; i < 3; ++i) {
        glm::vec3 const& pt = t.pts[i];
        push_pt(triangle_vertices, pt, color);
      }
      push_pt(triangle_normals, normal);

      push_pt(v_lines, t.pts[0]);
      push_pt(v_lines, t.pts[1]);
      push_pt(v_lines, t.pts[1]);
      push_pt(v_lines, t.pts[2]);
      push_pt(v_lines, t.pts[2]);
      push_pt(v_lines, t.pts[0]);

      // push_pt(v_lines, (t.pts[0] + t.pts[1] + t.pts[2]) / 3.0f);
      // push_pt(v_lines, normal + (t.pts[0] + t.pts[1] + t.pts[2]) / 3.0f);

      ++count;
    };

    intersect::Intersector().intersect(handle_triangle, transformed_hyperplane);
  }
}

void Renderer::send_triangles() {
  if (is_color) {
    glUseProgram(color_prog);
  } else {
    glUseProgram(light_prog);
  }

  glBindVertexArray(vao[0]);
  CHECK_GL();

  setup_basic_projection(light_loc.mv, light_loc.proj);
  if (!is_color) {
    setup_light();
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  CHECK_GL();

  glBufferData(GL_ARRAY_BUFFER, triangle_vertices.size() * sizeof(float),
               triangle_vertices.data(), GL_STREAM_DRAW);
  CHECK_GL();

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  CHECK_GL();

  glEnableVertexAttribArray(0);
  CHECK_GL();
}

void Renderer::send_lines() {
  glUseProgram(wireframe_prog);
  CHECK_GL();

  setup_basic_projection(wireframe_loc.mv, wireframe_loc.proj);

  glBindVertexArray(vao[0]);
  CHECK_GL();

  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  CHECK_GL();

  glBufferData(GL_ARRAY_BUFFER, v_lines.size() * sizeof(float), v_lines.data(),
               GL_STATIC_DRAW);
  CHECK_GL();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  CHECK_GL();
  glEnableVertexAttribArray(0);
  CHECK_GL();
}

void Renderer::setup_light() {
  // ---------

  glUniformMatrix3x4fv(light_loc.material_mat, 1, GL_FALSE,
                       glm::value_ptr(gold_material.mat));
  CHECK_GL();

  glUniform1f(light_loc.material_shine, gold_material.shine);
  CHECK_GL();

  glUniformMatrix3x4fv(light_loc.directional_mat, 1, GL_FALSE,
                       glm::value_ptr(directional_light.mat));
  CHECK_GL();

  glUniform3fv(light_loc.directional_direction, 1,
               glm::value_ptr(directional_light.direction));
  CHECK_GL();

  glUniform4fv(light_loc.global_ambient, 1, glm::value_ptr(global_ambient));
  CHECK_GL();

  glUniformMatrix4fv(light_loc.normal_matrix_transform, 1, GL_FALSE,
                     glm::value_ptr(normal_matrix_transform));
  CHECK_GL();

  // ---------

  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  CHECK_GL();

  glBufferData(GL_ARRAY_BUFFER, triangle_normals.size() * sizeof(float),
               triangle_normals.data(), GL_STREAM_DRAW);
  CHECK_GL();

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  CHECK_GL();

  glEnableVertexAttribArray(1);
  CHECK_GL();
}

void Renderer::draw_hud() {
  std::ostringstream hud;
  auto const& hp = hyperplane_manager.get_hyperplane();
  hud << "Hyperplane:\n"
         "Center: "
      << hp.pos
      << "\n"
         "Unit vectors and normal:\n"
      << hp.coord_system;
  gltSetText(text1, hud.str().c_str());
  gltBeginDraw();
  gltColor(0.4f, 0.4f, 0.4f, 0.4f);
  gltDrawText2D(text1, 0, 0, 1.0f);
  gltEndDraw();
}

void Renderer::render() {
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  CHECK_GL();

  // glfwGetFramebufferSize(window, &width, &height);
  // CHECK_GLFW("get frame buffer size");

  setup_vertices();

  send_triangles();

  glEnable(GL_DEPTH_TEST);
  CHECK_GL();

  glDepthFunc(GL_LEQUAL);
  CHECK_GL();

  glDrawArrays(GL_TRIANGLES, 0, triangle_vertices.size() / 3);
  CHECK_GL();

  send_lines();

  glDrawArrays(GL_LINES, 0, v_lines.size() / 2);
  CHECK_GL();

  draw_hud();
}
void Renderer::set_size(glm::ivec2 size) { window_size = size; }
}  // namespace viz