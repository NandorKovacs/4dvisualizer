#ifndef VIZ_ERRORS_H
#define VIZ_ERRORS_H

#include <GL/glew.h>

#include <functional>
#include <ostream>
#include <string>
#include <iostream>
#include "../debug.h"

void print_shader_log(GLuint shader);
void print_program_log(GLuint prog);
bool print_opengl_error();
void check_gl(std::string file, int line,
              std::function<bool()> extra_check = nullptr);

#define CHECK_GL(...) check_gl(__FILE__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)

void check_glfw(std::string message, std::string file, int line);

void debug_log(std::string message, std::string file, int line);

#define CHECK_GLFW(message) check_glfw(message, __FILE__, __LINE__)

#endif  // VIZ_ERRORS_H
