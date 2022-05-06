#include "camera_manager.h"

  CameraManager::CameraManager() {
    loc = glm::vec3(0.0, 0.0, -2.0);
    angle = glm::vec3(0.0, 0.0, -1.0);
    move_speed = glm::vec3(0.0, 0.0, 0.0);
  }

  void CameraManager::set_angle(glm::vec3 angle) {
    this->angle = angle;
  }
  void CameraManager::set_loc(glm::vec3 loc) {
    this->loc = loc;
  }
  void CameraManager::set_move_speed(glm::vec3 move_speed){
    this->move_speed = move_speed;
  }
  glm::vec3 CameraManager::get_move_speed() {
    return move_speed;
  }

  glm::mat4 CameraManager::get_transform() {
    return transform;
  }


  void CameraManager::calculate_transform() {
    //do some fun math;
  }

  void CameraManager::tick(double time) {
    angle = glm::normalize(angle);
    loc += (move_speed * angle) * (float)time;

    calculate_transform();
  }