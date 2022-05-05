#include "renderer.h"

#include <GL/glew.h>

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

#include "lib/errors.h"
#include "shader_loader.h"

void Renderer::setup_vertices() {
  float vertex_positions[108] = {
      -1.0f, -1.0f, -1.0f,                       // triangle 1 : begin
      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,   // triangle 1 : end
      1.0f,  1.0f,  -1.0f,                       // triangle 2 : begin
      -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,  // triangle 2 : end
      1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,
      1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
      -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,
      -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f};

  glGenVertexArrays(1, vao);
  CHECK_GL();

  glBindVertexArray(vao[0]);
  CHECK_GL();

  glGenBuffers(NUM_VBOs, vbo);
  CHECK_GL();

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  CHECK_GL();

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions,
               GL_STATIC_DRAW);
  CHECK_GL();
}

void Renderer::init(glm::ivec2 size) {
  prog = create_shader_program("vshader.glsl", "fshader.glsl");

  setup_vertices();
  window_size = size;
}

void Renderer::render(double currentTime) {
  ++roation_count;
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  CHECK_GL();
  // glClearColor(1.0, 1.0, 1.0, 1.0);
  // CHECK_GL();
  // glClear(GL_COLOR_BUFFER_BIT);
  // CHECK_GL();

  glUseProgram(prog);
  CHECK_GL();

  mvLoc = glGetUniformLocation(prog, "mv_matrix");
  CHECK_GL();

  projLoc = glGetUniformLocation(prog, "proj_matrix");
  CHECK_GL();

  // glfwGetFramebufferSize(window, &width, &height);
  // CHECK_GLFW("get frame buffer size");

  aspect = (float)window_size.x / (float)window_size.y;
  pMat = glm::perspective(1.04721f, aspect, 0.1f, 1000.0f);

  vMat = glm::translate(glm::mat4(1.0f), camera_loc);
  mMat = glm::translate(glm::mat4(1.0f), cube_loc);
  rotMat = glm::rotate(glm::mat4(1.0f), (float)(fmod(currentTime, 360)),
                       glm::vec3(0.0f, 1.0f, 0.0f));
  tiltMat = glm::rotate(glm::mat4(1.0f), 5.0f,
                       glm::vec3(1.0f, 0.0f, 1.0f));
  mMat = mMat * rotMat * tiltMat;

  mvMat = vMat * mMat;

  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
  CHECK_GL();
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
  CHECK_GL();
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  CHECK_GL();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  CHECK_GL();
  glEnableVertexAttribArray(0);
  CHECK_GL();

  glEnable(GL_DEPTH_TEST);
  CHECK_GL();
  glDepthFunc(GL_LEQUAL);
  CHECK_GL();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  CHECK_GL();
}

void Renderer::set_size(glm::ivec2 size) {
  window_size = size;
}