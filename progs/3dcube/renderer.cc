#include "renderer.h"

#include <GL/glew.h>

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <cmrc/cmrc.hpp>

#include "../../lib/camera_manager.h"
#include "../../lib/errors.h"
#include "../../lib/shader_loader.h"

CMRC_DECLARE(viz_3dcube_glsl);

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

Renderer::Renderer(glm::ivec2 window_size, CameraManager& camera_manager) : camera_manager{camera_manager}, window_size{window_size} {
  cmrc::embedded_filesystem fs = cmrc::viz_3dcube_glsl::get_filesystem();
  cmrc::file vshader = fs.open("vshader.glsl");
  cmrc::file fshader = fs.open("fshader.glsl");

  prog = create_shader_program(
    std::string{vshader.begin(), vshader.end()},
    std::string{fshader.begin(), fshader.end()});

  setup_vertices();
}

void Renderer::render() {
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

  vMat = camera_manager.get_transform();

  // std::cerr << "renderer.cc: vMat\n" << vMat << std::endl;

  mMat = glm::rotate(glm::mat4(1.0), 60.0f, glm::vec3(0, 1, 0));
  mMat = glm::translate(glm::mat4(1.0f), cube_loc) * mMat;
  

  // std::cout << "-------------" << std::endl;
  // DLOG << "vMat manager" << vMat << std::endl;
  // DLOG << "manager " << *camera_manager << std::endl;
  // vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  // DLOG << "vMat working" << vMat << std::endl;
  // std::cout << "-------------" << std::endl;


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
void Renderer::set_size(glm::ivec2 size) { window_size = size; }