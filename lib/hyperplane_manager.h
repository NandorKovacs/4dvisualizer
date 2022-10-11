#ifndef VIZ_HYPERPLANE_MANAGER_H
#define VIZ_HYPERPLANE_MANAGER_H

#include <glm/glm.hpp>
#include "../geometry/geometry.h"

namespace viz {

class HyperplaneManger {
 public:
  HyperplaneManger();

  void tick(double time);

  void set_move_direction(glm::vec3 move_direction);

  // more complicated than this, needs thinking
  void rotate(glm::vec2 movement);

  glm::vec3 get_move_directions();

 private:
  void move();

  Hyperplane hyperplane;
  
  glm::vec4 normal_vector_speed;
  int travel_direction; // can be 1, -1, and 0

  const float speed = 0.5;
};

}  // namespace viz
#endif  // VIZ_HYPERPLANE_MANAGER_H