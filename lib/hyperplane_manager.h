#ifndef VIZ_HYPERPLANE_MANAGER_H
#define VIZ_HYPERPLANE_MANAGER_H

#include <glm/glm.hpp>
#include "../geometry/geometry.h"

namespace viz {

class HyperplaneManager {
 public:
  HyperplaneManager();

  void tick(double time);

  void set_move_direction(float x);

  void set_rot_X(float x);
  void set_rot_Y(float y);
  void set_rot_Z(float z);

  glm::mat4 get_transform();
  glm::vec4& get_origin();

  Hyperplane& get_hyperplane();
 private:
  Hyperplane hyperplane;
  
  glm::vec3 rot_direction;
  void rotate(float rad);
  void rotate_axis(float rad, glm::vec4& axis);

  int move_direction;

  glm::mat4 transform;

  const float speed = 3.5;
  const float rot_speed = 0.5;
};

}  // namespace viz

#endif  // VIZ_HYPERPLANE_MANAGER_H