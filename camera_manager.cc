#include "camera_manager.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>


  CameraManager::CameraManager() {
    loc = glm::vec3(0.0, 0.0, -2.0);
    angle = glm::vec3(0.0, 0.0, -1.0);
    move_speed = glm::vec3(0.0, 0.0, 0.0);
  }

  void CameraManager::set_angle(glm::vec3 angle) {
    this->angle = angle;
  }
  glm::vec3 CameraManager::get_angle() {
    return angle;
  }
  void CameraManager::set_loc(glm::vec3 loc) {
    this->loc = loc;
  }
  glm::vec3 CameraManager::get_loc() {
    return loc;
  }
  void CameraManager::add_move_speed(glm::vec3 move_speed){
    this->move_speed = move_speed + this->move_speed;
  }
  glm::vec3 CameraManager::get_move_speed() {
    return move_speed;
  }

  glm::mat4 CameraManager::get_transform() {
    return transform;
  }

  void CameraManager::set_euler() {
    
  }

  void CameraManager::calculate_transform() {
    
    
    transform = * glm::translate(glm::mat4(1.0), loc);
  }

  void CameraManager::prev_event_tick() {
    move_speed = glm::vec3(0.0, 0.0, 0.0);
  }

  void CameraManager::post_event_tick(double time) {
    loc += (move_speed * angle) * (float)time;

    calculate_transform();
  }