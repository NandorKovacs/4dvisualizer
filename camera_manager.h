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

  void post_event_tick(double time);

  void set_move_direction(glm::vec3 move_direction);
  glm::vec3 get_move_directions();

  glm::mat4 get_transform();
  void calculate_transform();
 private:
  void set_euler();

  glm::vec3 loc;
  glm::vec3 angle;

  glm::vec3 move_directions;
  glm::mat4 transform;
};

#endif // VIZ_CAMERA_MANAGER