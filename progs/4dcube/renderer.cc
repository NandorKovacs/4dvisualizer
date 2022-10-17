#include "renderer.h"

#include <GL/glew.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>
#include <iostream>

#include "../../geometry/intersect.h"
#include "../../lib/camera_manager.h"
#include "../../lib/errors.h"
#include "../../lib/shader_loader.h"

namespace viz {
namespace {
void push_pt(std::vector<float>& f, glm::vec3 const& pt, float color) {
  f.push_back(pt.x);
  f.push_back(pt.y);
  f.push_back(pt.z);
  f.push_back(color);
};
}  // namespace

void Renderer::setup_vertices() {
  v_triangles.clear();
  v_lines.clear();
  int count = 0;
  
  auto handle_triangle = [&](intersect::Triangle const& t) {
    
    for (int i = 0; i < 3; ++i) {
      glm::vec3 const& pt = t.pts[i];
      push_pt(v_triangles, pt, count);
    }
    for (int i = 0; i < 3; ++i) {
      glm::vec3 const& pt = t.pts[2 - i];

      push_pt(v_triangles, pt, count);
    }

    push_pt(v_lines, t.pts[0],0);
    push_pt(v_lines, t.pts[1],0);
    push_pt(v_lines, t.pts[1],0);
    push_pt(v_lines, t.pts[2],0);
    push_pt(v_lines, t.pts[2],0);
    push_pt(v_lines, t.pts[0],0);

    ++count;
  };

  intersect::Intersector().intersect(handle_triangle,
                                     hyperplane_manager.get_hyperplane());
}

void Renderer::send_triangles() {
  glUniform1i(is_wireframe_loc, 0);
  CHECK_GL();

  glBindVertexArray(vao[0]);
  CHECK_GL();

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  CHECK_GL();

  glBufferData(GL_ARRAY_BUFFER, v_triangles.size() * sizeof(float),
               v_triangles.data(), GL_STATIC_DRAW);
  CHECK_GL();

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, 0, 0);
  CHECK_GL();
  glEnableVertexAttribArray(0);
  CHECK_GL();
}

void Renderer::send_lines() {
  glUniform1i(is_wireframe_loc, 1);
  CHECK_GL();

  glBindVertexArray(vao[0]);
  CHECK_GL();

  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  CHECK_GL();

  glBufferData(GL_ARRAY_BUFFER, v_lines.size() * sizeof(float), v_lines.data(),
               GL_STATIC_DRAW);
  CHECK_GL();

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, 0, 0);
  CHECK_GL();
  glEnableVertexAttribArray(0);
  CHECK_GL();
}

Renderer::Renderer(glm::ivec2 window_size, CameraManager& camera_manager,
                   HyperplaneManager& hyperplane_manager)
    : camera_manager{camera_manager},
      window_size{window_size},
      hyperplane_manager{hyperplane_manager} {
  prog = create_shader_program("vshader.glsl", "fshader.glsl");

  glGenVertexArrays(1, vao);
  CHECK_GL();

  glGenBuffers(NUM_VBOs, vbo);
  CHECK_GL();
}

void Renderer::render() {
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  CHECK_GL();

  glUseProgram(prog);
  CHECK_GL();

  mvLoc = glGetUniformLocation(prog, "mv_matrix");
  CHECK_GL();
  projLoc = glGetUniformLocation(prog, "proj_matrix");
  CHECK_GL();
  is_wireframe_loc = glGetUniformLocation(prog, "is_wireframe");
  CHECK_GL();

  // glfwGetFramebufferSize(window, &width, &height);
  // CHECK_GLFW("get frame buffer size");

  aspect = (float)window_size.x / (float)window_size.y;
  pMat = glm::perspective(1.04721f, aspect, 0.1f, 1000.0f);

  glm::mat4& vMat = camera_manager.get_transform();

  mMat = glm::rotate(glm::mat4(1.0), 60.0f, glm::vec3(0, 1, 0));
  mMat = glm::translate(glm::mat4(1.0f), cube_loc) * mMat;

  mvMat = vMat * mMat;

  glm::vec4& origin = hyperplane_manager.get_origin();

  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
  CHECK_GL();
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
  CHECK_GL();

  setup_vertices();
  send_triangles();

  glEnable(GL_DEPTH_TEST);
  CHECK_GL();
  glDepthFunc(GL_LEQUAL);
  CHECK_GL();
  // glEnable(GL_BLEND);
  // CHECK_GL();

  glDrawArrays(GL_TRIANGLES, 0, v_triangles.size());
  CHECK_GL();

  send_lines();
  glDrawArrays(GL_LINES, 0, v_lines.size());
  CHECK_GL();
}
void Renderer::set_size(glm::ivec2 size) { window_size = size; }
}  // namespace viz