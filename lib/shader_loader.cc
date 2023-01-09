#include "shader_loader.h"

#include <GL/glew.h>

#include <string>
#include <iostream>
#include <exception>

#include "errors.h"

GLuint create_shader_program(std::string vshader_content, std::string fshader_content) {
  GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
  CHECK_GL();
  GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
  CHECK_GL();

  const char* shader_source_array[1];

  shader_source_array[0] = vshader_content.c_str();
  glShaderSource(vshader, 1, shader_source_array, NULL);
  CHECK_GL();

  shader_source_array[0] = fshader_content.c_str();
  glShaderSource(fshader, 1, shader_source_array, NULL);
  CHECK_GL();

  glCompileShader(vshader);
  CHECK_GL_EXTRA([&]() {
    GLint num_compiled = 0;
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &num_compiled);
    CHECK_GL();
    if (num_compiled != 1) {
      std::cerr << "Vertex compilation failed. ";
      print_shader_log(vshader);
      return true;
    }
    return false;
  });

  glCompileShader(fshader);
  CHECK_GL_EXTRA([&]() {
    GLint num_compiled = 0;
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &num_compiled);
    CHECK_GL();
    if (num_compiled == 1) {
      return false;
    }
    std::cerr << "Fragment compilation failed. ";
    print_shader_log(fshader);
    return true;
  });

  GLuint prog = glCreateProgram();
  CHECK_GL();

  glAttachShader(prog, vshader);
  CHECK_GL();

  glAttachShader(prog, fshader);
  CHECK_GL();

  glLinkProgram(prog);
  CHECK_GL_EXTRA([&]() {
    GLint linked;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (linked) {
      return false;
    }
    std::cerr << "Linking failed. ";
    print_program_log(prog);
    return true;
  });

  return prog;
}
