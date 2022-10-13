#include "renderer.h"

#include <GL/glew.h>

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

#include "../../geometry/intersect.h"
#include "../../lib/camera_manager.h"
#include "../../lib/errors.h"
#include "../../lib/shader_loader.h"

namespace viz {
void Renderer::setup_vertices() {
  vertices.clear();
  auto handle_triangle = [&](intersect::Triangle const& t) {
    for (int i = 0; i < 3; ++i) {
      glm::vec4 const& pt = t.pts[i];

      vertices.push_back(pt.x);
      vertices.push_back(pt.y);
      vertices.push_back(pt.z);
      vertices.push_back(pt.w);
    }
    for (int i = 0; i < 3; ++i) {
      glm::vec4 const& pt = t.pts[3-i];

      vertices.push_back(pt.x);
      vertices.push_back(pt.y);
      vertices.push_back(pt.z);
      vertices.push_back(pt.w);
    }
  };

  intersect::Intersector().intersect(handle_triangle, hyperplane_manager.get_hyperplane());
}

void Renderer::send_vertices() {
  glBindVertexArray(vao[0]);
  CHECK_GL();

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  CHECK_GL();

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);
  CHECK_GL();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
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
  dim_proj_loc = glGetUniformLocation(prog, "dim_proj_matrix");
  CHECK_GL();
  dim_proj_origin_loc = glGetUniformLocation(prog, "dim_proj_origin");
  CHECK_GL();

  // glfwGetFramebufferSize(window, &width, &height);
  // CHECK_GLFW("get frame buffer size");

  aspect = (float)window_size.x / (float)window_size.y;
  pMat = glm::perspective(1.04721f, aspect, 0.1f, 1000.0f);

  glm::mat4& vMat = camera_manager.get_transform();

  mMat = glm::rotate(glm::mat4(1.0), 60.0f, glm::vec3(0, 1, 0));
  mMat = glm::translate(glm::mat4(1.0f), cube_loc) * mMat;

  mvMat = vMat * mMat;

  dim_proj_mat = hyperplane_manager.get_transform();

  glm::vec4& origin = hyperplane_manager.get_origin();

  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
  CHECK_GL();
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
  CHECK_GL();
  glUniformMatrix4fv(dim_proj_loc, 1, GL_FALSE, glm::value_ptr(dim_proj_mat));
  CHECK_GL();
  glUniform4fv(dim_proj_origin_loc, 1, glm::value_ptr(origin));
  CHECK_GL();

  setup_vertices();
  send_vertices();

  glEnable(GL_DEPTH_TEST);
  CHECK_GL();
  glDepthFunc(GL_LEQUAL);
  CHECK_GL();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  CHECK_GL();
}
void Renderer::set_size(glm::ivec2 size) { window_size = size; }
}  // namespace viz