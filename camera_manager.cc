#include "camera_manager.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

#include "lib/errors.h"

CameraManager::CameraManager()
    : pitch{0.0},
      yaw{0.0f},
      loc{glm::vec3(0.0, 0.0, 0.0)},
      move_directions{glm::vec3(0.0, 0.0, 0.0)} {}

void CameraManager::set_loc(glm::vec3 loc) { this->loc = loc; }

glm::vec3 CameraManager::get_loc() { return loc; }

void CameraManager::set_move_direction(glm::vec3 move_direction) {
  this->move_directions = move_direction;
}

void CameraManager::rotate(glm::vec2 movement) {
  pitch += movement.y * turn_speed;
  yaw += movement.x * turn_speed;
}

glm::vec3 CameraManager::get_move_directions() { return move_directions; }

glm::mat4 CameraManager::get_transform() { return transform; }

void CameraManager::calculate_transform() {
  transform = glm::eulerAngleXY(-pitch, -yaw) *
              glm::translate(glm::mat4(1.0), (loc * -1.0f));
}

void CameraManager::move() {
  glm::vec3 move_vec =
      glm::rotate(glm::normalize(move_directions), yaw, glm::vec3(0, 1, 0)) *
      speed;
  loc += -move_vec * speed;
}

void CameraManager::tick(double time) {
  if (move_directions != glm::vec3(0, 0, 0)) {
    move();
  }

  calculate_transform();
}