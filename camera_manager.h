#ifndef VIZ_CAMERA_MANAGER
#define VIZ_CAMERA_MANAGER

#include "glm/glm.hpp"
#include "lib/errors.h"

class CameraManager {
 public:
  CameraManager();

  void set_loc(glm::vec3 loc);

  void tick(double time);

  void set_move_direction(glm::vec3 move_direction);
  void rotate(glm::vec2 movement);

  glm::mat4 get_transform();

  glm::vec3 get_move_directions();

 private:
  friend std::ostream& operator<<(std::ostream& os, CameraManager m);

  void move();
  void calculate_transform();

  glm::vec3 get_loc();
  float pitch, yaw;

  glm::vec3 loc;

  glm::vec3 move_directions;
  glm::mat4 transform;

  const float speed = 0.5;
  const float turn_speed = 0.001;
};

inline std::ostream& operator<<(std::ostream& os, CameraManager m) {
  os << "{"
     << "\nloc: " << m.loc << "\npitch: " << m.pitch << "\nyaw: " << m.yaw
     << "\nmove_directions: " << m.move_directions
     << "\ntransform: " << m.transform << "\nspeed: " << m.speed << "\n}";
  return os;
}
#endif  // VIZ_CAMERA_MANAGER