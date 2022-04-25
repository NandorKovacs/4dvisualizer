#include "renderer.h"

#include "shader_loader.h"

#include "lib/errors.h"

void Renderer::init() {
  prog = create_shader_program("vshader.glsl", "fshader.glsl");
  glUseProgram(prog);
  CHECK_GL();
  glGenVertexArrays(NUM_VAOs, vao);
  glBindVertexArray(vao[0]);
  glEnable(GL_PROGRAM_POINT_SIZE);
}

void Renderer::render() {
  glPointSize(30.0f);
  glDrawArrays(GL_POINTS, 0, 1);
}