#include "errors.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <iostream>
#include <string>
#include <vector>

void print_shader_log(GLuint shader) {
  GLint len = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
  if (len == 0) {
    return;
  }

  std::vector<char> buffer(len);
  glGetShaderInfoLog(shader, len, NULL, buffer.data());
  std::cerr << "Shader Info Log: " << buffer.data() << std::endl;
}

void print_program_log(GLuint prog) {
  GLint len = 0;
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
  if (len == 0) {
    return;
  }

  std::vector<char> buffer(len);
  glGetProgramInfoLog(prog, len, NULL, buffer.data());
  std::cerr << "Program Info Log: " << buffer.data() << std::endl;
}

bool print_opengl_error(std::string file, int line) {
  bool foundError = false;
  GLenum glErr = glGetError();
  while (glErr != GL_NO_ERROR) {
    std::cerr << "glError: " << file << ":" << line << " " << glErr
#ifndef __APPLE__
              << ": " << glewGetErrorString(glErr)
#endif
              << std::endl;
    foundError = true;
    glErr = glGetError();
  }
  return foundError;
}

void check_gl(std::string file, int line, std::function<bool()> extra_check) {
  bool has_error = print_opengl_error(file, line);
  bool has_extra_check_error = false;
  if (extra_check != nullptr) {
    has_extra_check_error = extra_check();
  }
  if (has_error || has_extra_check_error) {
    std::terminate();
  }
  if (extra_check != nullptr) {
    // We need to check again, in case within the extra check we ran into
    // some OpenGL errors.
    CHECK_GL();
  }
}

void check_glfw(std::string message, std::string file, int line) {
  const char* error_string;
  if (glfwGetError(&error_string)) {
    std::cerr << "GLFW error: " << file << ":" << line << " " << message << ": "
              << (error_string ? error_string : "unknown error") << std::endl;
    std::terminate();
  }
}

void debug_log(std::string message, std::string file, int line) {
  std::cerr << file << ", line " << line << ": " << message << std::endl;
}