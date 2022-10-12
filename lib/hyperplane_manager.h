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
  void set_rot_W(float w);

 private:
  Hyperplane hyperplane;
  
  glm::vec4 normal_vector_speed; // can all be -1 to 1
  int move_direction; // can be -1 to 1

  const float speed = 0.5;
  const float rot_speed = 0.5;
};

}  // namespace viz
#endif  // VIZ_HYPERPLANE_MANAGER_H