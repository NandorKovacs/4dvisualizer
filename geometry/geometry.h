#ifndef VIZ_GEOMETRY_H
#define VIZ_GEOMETRY_H

#include <glm/glm.hpp>

namespace viz {

struct Hyperplane {
  Hyperplane() = default;
  Hyperplane(glm::vec4 normal, glm::vec4 pos) : pos{pos}, normal{normal} {}

  glm::vec4 normal;
  glm::vec4 pos;
};

}  // namespace viz

#endif  // VIZ_GEOMETRY_H