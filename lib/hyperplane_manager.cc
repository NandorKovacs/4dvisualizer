#include "hyperplane_manager.h"

#include "errors.h"

namespace viz {
HyperplaneManager::HyperplaneManager()
    : hyperplane{glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 1)},
      normal_vector_speed{glm::vec4(0, 0, 0, 0)},
      move_direction{0} {}

void HyperplaneManager::tick(double time) {
  hyperplane.normal += normal_vector_speed * float(rot_speed * time);
  hyperplane.pos += hyperplane.normal * float(speed * time * move_direction);
}

void HyperplaneManager::set_move_direction(float x) {
  DLOG << "move hyperplane" << std::endl;
}

void HyperplaneManager::set_rot_X(float x) {
  DLOG << "plane rot x" << std::endl;
}
void HyperplaneManager::set_rot_Y(float y) {
  DLOG << "plane rot y" << std::endl;
}
void HyperplaneManager::set_rot_Z(float z) {
  DLOG << "plane rot z" << std::endl;
}
void HyperplaneManager::set_rot_W(float w) {
  DLOG << "plane rot w" << std::endl;
}

}  // namespace viz