#include "hyperplane_manager.h"

#include "errors.h"

namespace viz {
HyperplaneManager::HyperplaneManager()
    : hyperplane{glm::vec4(0, 0, 0, 0),
                 glm::mat4{glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
                           glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1)}},
      rot_direction{glm::vec3(0, 0, 0)},
      move_direction{0} {}

void HyperplaneManager::tick(double time) {
  rotate(time * rot_speed);

  if (move_direction != 0) {
    hyperplane.pos +=
        hyperplane.normal() * float(speed * time * move_direction);
  }

  DLOG << "normal: " << hyperplane.normal() << std::endl;
  DLOG << "pos: " << hyperplane.pos << std::endl;
  std::cerr << "------" << std::endl;
}

void HyperplaneManager::rotate_axis(float rad, glm::vec4& axis) {
  glm::vec4 new_normal =
      hyperplane.normal() * glm::cos(rad) + axis * glm::sin(rad);
  glm::vec4 new_axis =
      hyperplane.normal() * -glm::sin(rad) + axis * glm::cos(rad);

  hyperplane.normal() = new_normal;
  axis = new_axis;
}

void HyperplaneManager::rotate(float rad) {
  if (rot_direction.x != 0) {
    rotate_axis(rad * rot_direction.x, hyperplane.unit_x());
  }
  if (rot_direction.y != 0) {
    rotate_axis(rad * rot_direction.y, hyperplane.unit_y());
  }
  if (rot_direction.z != 0) {
    rotate_axis(rad * rot_direction.z, hyperplane.unit_z());
  }
}

void HyperplaneManager::set_move_direction(float x) { move_direction = x; }

void HyperplaneManager::set_rot_X(float x) { rot_direction.x = x; }
void HyperplaneManager::set_rot_Y(float y) { rot_direction.y = y; }
void HyperplaneManager::set_rot_Z(float z) { rot_direction.z = z; }

glm::mat4 HyperplaneManager::get_transform() {
  return glm::transpose(hyperplane.coord_system);
}
glm::vec4& HyperplaneManager::get_origin() { return hyperplane.pos; }

Hyperplane& HyperplaneManager::get_hyperplane() { return hyperplane; };

}  // namespace viz