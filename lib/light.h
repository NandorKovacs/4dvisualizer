#ifndef VIZ_LIGHT_H
#define VIZ_LIGHT_H
namespace viz {

#include <glm/glm.hpp>

struct Material {
  glm::mat3x4 mat;
  float shine;
};

struct DirectionalLight {
  glm::mat3x4 mat;
  glm::vec3 direction;
};

}  // namespace viz

#endif  // VIZ_LIGHT_H