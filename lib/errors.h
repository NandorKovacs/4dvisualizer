#ifndef VIZ_ERRORS_H
#define VIZ_ERRORS_H

#include <GL/glew.h>

#include <functional>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <ostream>
#include <string>

void print_shader_log(GLuint shader);
void print_program_log(GLuint prog);
bool print_opengl_error();
void check_gl(std::string file, int line,
              std::function<bool()> extra_check = nullptr);

#define CHECK_GL(...) check_gl(__FILE__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)

template <typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, glm::vec<4, T, Q> const& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
  return os;
}

template <typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, glm::vec<3, T, Q> const& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}

template <typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, glm::vec<2, T, Q> const& v) {
  os << "(" << v.x << ", " << v.y << ")";
  return os;
}

template <typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, glm::mat<4, 4, T, Q> const& m) {
  auto v = glm::value_ptr(m);
  os << "(" << std::endl;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      os << v[4 * i + j] << " ";
    }
    os << std::endl;
  }
  os << ")";
  return os;
}


void check_glfw(std::string message, std::string file, int line);

#define CHECK_GLFW(message) check_glfw(message, __FILE__, __LINE__)

#endif  // VIZ_ERRORS_H
