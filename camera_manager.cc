#include "camera_manager.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>


  CameraManager::CameraManager() {
    loc = glm::vec3(0.0, 0.0, -2.0);
    angle = glm::vec3(0.0, 0.0, -1.0);
    move_directions = glm::vec3(0.0, 0.0, 0.0);
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
  void CameraManager::set_move_direction(glm::vec3 move_direction){
    this->move_directions = move_direction;
  }
  glm::vec3 CameraManager::get_move_directions() {
    return move_directions;
  }

  glm::mat4 CameraManager::get_transform() {
    return transform;
  }

  void CameraManager::set_euler() {
    
  }

  void CameraManager::calculate_transform() {
    
    
    transform = glm::translate(glm::mat4(1.0), loc);
  }

  void CameraManager::post_event_tick(double time) {

    calculate_transform();
  }