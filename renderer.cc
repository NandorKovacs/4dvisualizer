#include "renderer.h"
#include <GL/glew.h>
#include "shader_loader.h"
#include <iostream>

#include "lib/errors.h"

void Renderer::init() {
  prog = create_shader_program("vshader.glsl", "fshader.glsl");
  glUseProgram(prog);
  CHECK_GL();
  
  glGenVertexArrays(NUM_VAOs, vao);
  CHECK_GL();

  glBindVertexArray(vao[0]);
  CHECK_GL();

  glEnable(GL_PROGRAM_POINT_SIZE);
  CHECK_GL();
}

void Renderer::render() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

    if (offset > max_offset) {
      direction = false;
    } else if (offset < max_offset * -1){
      direction = true;
    }
    if (direction) {
      offset += inc;
    } else {
      offset -= inc;
    }

    GLuint offsetLoc = glGetUniformLocation(prog, "offset");
    CHECK_GL();

    glProgramUniform1f(prog, offsetLoc, offset);
    CHECK_GL();


  glPointSize(30.0f);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}