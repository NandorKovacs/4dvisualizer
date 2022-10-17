#ifndef VIZ_DEBUG_H
#define VIZ_DEBUG_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <ostream>

template <typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, glm::vec<4, T, Q> const& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
  return os;
}

template <typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, glm::vec<3, T, Q> const& v) {
  os << "( " << v.x << " " << v.y << " " << v.z << " )";
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

#define DLOG std::cerr << __FILE__ << ", line " << __LINE__ << ": "

#endif  // VIZ_DEBUG_H