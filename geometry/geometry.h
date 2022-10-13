#ifndef VIZ_GEOMETRY_H
#define VIZ_GEOMETRY_H

#include <array>
#include <glm/glm.hpp>

namespace viz {

struct Hyperplane {
  Hyperplane() = default;
  Hyperplane(glm::vec4 pos, glm::mat4 coord_system)
      : pos{pos}, coord_system{coord_system} {}

  inline glm::vec4& normal() { return coord_system[3]; }
  inline glm::vec4& unit_x() { return coord_system[0]; }
  inline glm::vec4& unit_y() { return coord_system[1]; }
  inline glm::vec4& unit_z() { return coord_system[2]; }
  glm::vec4 pos;

  // The column vectors are equivalent to the unit vectors
  glm::mat4 coord_system;
};

}  // namespace viz

#endif  // VIZ_GEOMETRY_H