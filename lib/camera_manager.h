#ifndef VIZ_CAMERA_MANAGER_H
#define VIZ_CAMERA_MANAGER_H

#include "glm/glm.hpp"
#include "errors.h"

class CameraManager {
 public:
  CameraManager();

  // void set_loc(glm::vec3 loc);

  void tick(double time);

  void set_move_X(double x);
  void set_move_Y(double y);
  void set_move_Z(double z);

  void rotate(glm::vec2 movement);

  glm::mat4& get_transform();

 private:
  friend std::ostream& operator<<(std::ostream& os, CameraManager m);

  void move(double time);
  void calculate_transform();

  // glm::vec3 get_loc();
  float pitch, yaw;

  glm::vec3 loc;

  glm::vec3 move_directions;
  glm::mat4 transform;

  const float speed = 3.5;
  const float turn_speed = 0.001;
};

inline std::ostream& operator<<(std::ostream& os, CameraManager m) {
  os << "{"
     << "\nloc: " << m.loc << "\npitch: " << m.pitch << "\nyaw: " << m.yaw
     << "\nmove_directions: " << m.move_directions
     << "\ntransform: " << m.transform << "\nspeed: " << m.speed << "\n}";
  return os;
}
#endif  // VIZ_CAMERA_MANAGER_H
