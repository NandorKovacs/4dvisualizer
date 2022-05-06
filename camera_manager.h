#ifndef VIZ_CAMERA_MANAGER
#define VIZ_CAMERA_MANAGER

#include "glm/glm.hpp";

class CameraManager {
 public:
  CameraManager();

  void set_angle(glm::vec3 angle);
  glm::vec3 get_angle();
  void set_loc(glm::vec3 loc);
  glm::vec3 get_loc();
  void tick(double time);

  void set_move_speed(glm::vec3 move_speed);
  glm::vec3 get_move_speed();

  glm::mat4 get_transform();
 private:
  glm::vec3 loc;
  glm::vec3 angle;

  glm::vec3 move_speed;

  void calculate_transform();
  glm::mat4 transform;
};

#endif // VIZ_CAMERA_MANAGER