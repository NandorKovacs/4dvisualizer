#include "camera_manager.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

#include "errors.h"

CameraManager::CameraManager()
    : pitch{0.0},
      yaw{0.0f},
      loc{glm::vec3(0.0, 0.0, 0.0)},
      move_directions{glm::vec3(0.0, 0.0, 0.0)} {}

// void CameraManager::set_loc(glm::vec3 loc) { this->loc = loc; }

// glm::vec3 CameraManager::get_loc() { return loc; }


void CameraManager::set_move_X(double x) {
  move_directions.x = x;
}
void CameraManager::set_move_Y(double y) {
  move_directions.y = y;
}
void CameraManager::set_move_Z(double z) {
  move_directions.z = z;
}

void CameraManager::rotate(glm::vec2 rot_movement) {
  pitch += rot_movement.y * turn_speed;
  yaw += rot_movement.x * turn_speed;
}

glm::mat4& CameraManager::get_transform() { return transform; }

void CameraManager::calculate_transform() {
  transform = glm::eulerAngleXY(-pitch, -yaw) *
              glm::translate(glm::mat4(1.0), (loc * -1.0f));
}

void CameraManager::move(double time) {
  glm::vec3 move_vec =
      glm::rotate(glm::normalize(move_directions), yaw, glm::vec3(0, 1, 0)) *
      speed;
  loc += -move_vec * float(speed * time);
}

void CameraManager::tick(double time) {
  if (move_directions != glm::vec3(0, 0, 0)) {
    move(time);
  }

  calculate_transform();
}