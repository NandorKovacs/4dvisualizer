#ifndef VIZ_CAMERA_MANAGER
#define VIZ_CAMERA_MANAGER

#include "glm/glm.hpp";

class CameraManager {
 public:
  CameraManager();

  void set_angle(glm::vec3 angle);
  void set_loc(glm::vec3 loc);
  

  void tick(double time);

  void set_move_direction(glm::vec3 move_direction);

  glm::mat4 get_transform();
 
  glm::vec3 get_move_directions();
 private:
  void move();
  void calculate_transform();

  glm::vec3 get_angle();
  glm::vec3 get_loc();

  glm::vec3 loc;
  glm::vec3 angle;

  glm::vec3 move_directions;
  glm::mat4 transform;

  const float speed = 1.0;
};

#endif // VIZ_CAMERA_MANAGER