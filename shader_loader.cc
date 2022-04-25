#include "shader_loader.h"

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>

#include "lib/errors.h"

std::string load_file(std::string file_name) {
  std::ifstream f;
  f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    f.open(file_name);
  } catch (std::ios_base::failure& e) {
    std::cerr << "Error opening " << file_name << ": " << e.what() << std::endl;
    std::abort();
  }

  std::ostringstream buffer;
  try {
    buffer << f.rdbuf();
  } catch (std::ios_base::failure& e) {
    std::cerr << "Error reading " << file_name << ": " << e.what() << std::endl;
    std::abort();
  }
  return buffer.str();
}

GLuint create_shader_program(std::string vshader_file, std::string fshader_file) {
  std::string vshader_content = load_file(vshader_file);
  std::string fshader_content = load_file(fshader_file);

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
  CHECK_GL([&]() {
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
  CHECK_GL([&]() {
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
  CHECK_GL([&]() {
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
