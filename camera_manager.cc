#include "camera_manager.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "lib/errors.h"

CameraManager::CameraManager() {
  loc = glm::vec3(0.0, 0.0, -2.0);
  angle = glm::vec3(0.0, 0.0, -1.0);
  move_directions = glm::vec3(0.0, 0.0, 0.0);
}

void CameraManager::set_angle(glm::vec3 angle) { this->angle = angle; }
glm::vec3 CameraManager::get_angle() { return angle; }
void CameraManager::set_loc(glm::vec3 loc) { this->loc = loc; }
glm::vec3 CameraManager::get_loc() { return loc; }
void CameraManager::set_move_direction(glm::vec3 move_direction) {
  this->move_directions = move_direction;
}
glm::vec3 CameraManager::get_move_directions() { return move_directions; }

glm::mat4 CameraManager::get_transform() { return transform; }

void CameraManager::calculate_transform() {
  transform = glm::translate(glm::mat4(1.0), (loc * -1.0f));
  std::cerr << "transform: " << transform << "------" << std::endl;
  glm::mat4 rot = glm::rotate(
      glm::mat4(1.0),
      -1 * glm::acos(glm::dot(glm::normalize(glm::vec3(angle.x, 0, angle.y)),
                              glm::vec3(1.0, 0.0, 0.0))),
      glm::vec3(0, 1, 0));
  std::cerr << "rot: " << rot << "------" << std::endl;
  rot = glm::rotate(rot,
                    glm::acos(glm::dot(
                        glm::normalize(glm::vec3(angle.x, 0, angle.y)), angle)),
                    glm::vec3(0, 0, 1));
  std::cerr << "rot2: " << rot << "------" << std::endl;
  transform = transform * rot;
  std::cerr << "transform2: " << transform << "------" << std::endl;
}

void CameraManager::move() {
  glm::vec3 move_vec = glm::normalize(move_directions) * speed;
  glm::mat4 rot_mat = glm::rotate(
      glm::mat4(1.0),
      glm::acos(glm::dot(glm::vec3(1.0, 0.0, 0),
                         glm::normalize(glm::vec3(angle.x, 0, angle.y)))),
      glm::vec3(0, 1, 0));
  glm::vec4 res_vec = glm::vec4(move_vec, 0) * rot_mat;
  move_vec = glm::vec3(res_vec.x, res_vec.y, res_vec.z);
  loc += move_vec * speed;
}

// ! be aware of radiant <-> degree bugs

void CameraManager::tick(double time) {
  if (move_directions != glm::vec3(0, 0, 0)) {
    move();
  }

  calculate_transform();
}