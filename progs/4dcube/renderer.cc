#include "renderer.h"

#include <GL/glew.h>

#include <cmath>
#include <cmrc/cmrc.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>
#include <iostream>

#include "../../geometry/intersect.h"
#include "../../lib/camera_manager.h"
#include "../../lib/errors.h"
#include "../../lib/shader_loader.h"

CMRC_DECLARE(viz_4dcube_glsl);

namespace viz {

Renderer::Renderer(glm::ivec2 window_size, CameraManager& camera_manager,
                   HyperplaneManager& hyperplane_manager)
    : camera_manager{camera_manager},
      window_size{window_size},
      hyperplane_manager{hyperplane_manager} {
  cmrc::embedded_filesystem fs = cmrc::viz_4dcube_glsl::get_filesystem();

  cmrc::file vshader = fs.open("vshader.glsl");
  cmrc::file fshader = fs.open("fshader.glsl");
  prog = create_shader_program(std::string{vshader.begin(), vshader.end()},
                               std::string{fshader.begin(), fshader.end()});

  cmrc::file wireframe_vshader = fs.open("wireframe_vshader.glsl");
  cmrc::file wireframe_fshader = fs.open("wireframe_fshader.glsl");
  wireframe_prog = create_shader_program(
      std::string{wireframe_vshader.begin(), wireframe_vshader.end()},
      std::string{wireframe_fshader.begin(), wireframe_fshader.end()});

  glGenVertexArrays(1, vao);
  CHECK_GL();

  glGenBuffers(NUM_VBOs, vbo);
  CHECK_GL();
}

void Renderer::setup_aspect_ratio(GLuint const& program) {
  aspect = (float)window_size.x / (float)window_size.y;
  pMat = glm::perspective(1.04721f, aspect, 0.1f, 1000.0f);

  glm::mat4& vMat = camera_manager.get_transform();

  mMat = glm::rotate(glm::mat4(1.0), 60.0f, glm::vec3(0, 1, 0));
  mMat = glm::translate(glm::mat4(1.0f), cube_loc) * mMat;

  mvMat = vMat * mMat;

  glm::vec4& origin = hyperplane_manager.get_origin();

  mvLoc = glGetUniformLocation(program, "mv_matrix");
  CHECK_GL();
  projLoc = glGetUniformLocation(program, "proj_matrix");
  CHECK_GL();

  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
  CHECK_GL();
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
  CHECK_GL();
}

namespace {
void push_pt(std::vector<float>& f, glm::vec3 const& pt) {
  f.push_back(pt.x);
  f.push_back(pt.y);
  f.push_back(pt.z);
  // f.push_back(color);
};
}  // namespace

void Renderer::setup_vertices() {
  triangle_vertices.clear();
  triangle_normals.clear();
  v_lines.clear();
  int count = 0;

  auto handle_triangle = [&](intersect::Triangle const& t,
                             glm::vec3 const& normal) {
    for (int i = 0; i < 3; ++i) {
      glm::vec3 const& pt = t.pts[i];
      push_pt(triangle_vertices, pt);
    }
    push_pt(triangle_normals, normal);

    push_pt(v_lines, t.pts[0]);
    push_pt(v_lines, t.pts[1]);
    push_pt(v_lines, t.pts[1]);
    push_pt(v_lines, t.pts[2]);
    push_pt(v_lines, t.pts[2]);
    push_pt(v_lines, t.pts[0]);
    
    push_pt(v_lines, (t.pts[0] + t.pts[1] + t.pts[2]) / 3.0f);
    push_pt(v_lines, normal + (t.pts[0] + t.pts[1] + t.pts[2]) / 3.0f);
    
    ++count;
  };

  intersect::Intersector().intersect(handle_triangle,
                                     hyperplane_manager.get_hyperplane());
}

void Renderer::send_triangles() {
  glUseProgram(prog);

  glBindVertexArray(vao[0]);
  CHECK_GL();

  setup_aspect_ratio(prog);
  setup_light();

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  CHECK_GL();

  glBufferData(GL_ARRAY_BUFFER, triangle_vertices.size() * sizeof(float),
               triangle_vertices.data(), GL_STATIC_DRAW);
  CHECK_GL();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  CHECK_GL();
  glEnableVertexAttribArray(0);
  CHECK_GL();
}

void Renderer::send_lines() {
  glUseProgram(wireframe_prog);
  CHECK_GL();
  setup_aspect_ratio(wireframe_prog);

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
  material_mat_loc = glGetUniformLocation(prog, "material.mat");
  CHECK_GL();
  material_shine_loc = glGetUniformLocation(prog, "material.shine");
  CHECK_GL();

  global_ambient_loc = glGetUniformLocation(prog, "global_ambient");
  CHECK_GL();

  directional_mat_loc = glGetUniformLocation(prog, "directional_light.mat");
  CHECK_GL();
  directional_direction_loc =
      glGetUniformLocation(prog, "directional_light.direction");
  CHECK_GL();

  normal_matrix_transform_loc =
      glGetUniformLocation(prog, "normal_matrix_transform");
  CHECK_GL();

  // ---------

  glUniformMatrix3x4fv(material_mat_loc, 1, GL_FALSE,
                       glm::value_ptr(gold_material.mat));
  CHECK_GL();
  glUniform1f(material_shine_loc, 0.0f);
  CHECK_GL();

  glUniform4fv(global_ambient_loc, 1, glm::value_ptr(global_ambient));
  CHECK_GL();

  glUniformMatrix3x4fv(directional_mat_loc, 1, GL_FALSE,
                       glm::value_ptr(directional_light.mat));
  CHECK_GL();
  glUniform3fv(directional_direction_loc, 1,
               glm::value_ptr(directional_light.direction));
  CHECK_GL();

  glUniformMatrix4fv(normal_matrix_transform_loc, 1, GL_FALSE,
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

  glDrawArrays(GL_TRIANGLES, 0, triangle_vertices.size());
  CHECK_GL();

  send_lines();

  glDrawArrays(GL_LINES, 0, v_lines.size());
  CHECK_GL();
}
void Renderer::set_size(glm::ivec2 size) { window_size = size; }
}  // namespace viz