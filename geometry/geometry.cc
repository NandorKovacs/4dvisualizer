#include "geometry.h"

namespace viz {

glm::vec4 Hyperplane::to_abs(glm::vec3 pt) const {
  return (unit_x() * pt[0] + unit_y() * pt[1] + unit_z() * pt[2]) + pos;
}

glm::vec3 Hyperplane::to_rel(glm::vec4 pt) const {
  glm::vec3 res;

  glm::vec4 rel_pt = pos - pt;
  res[0] = glm::dot(unit_x(), rel_pt) / glm::length(unit_x());
  res[1] = glm::dot(unit_y(), rel_pt) / glm::length(unit_y());
  res[2] = glm::dot(unit_z(), rel_pt) / glm::length(unit_z());
  return res;
}


}  // namespace viz
