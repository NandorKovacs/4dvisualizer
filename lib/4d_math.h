#ifndef VIZ_4D_MATH_H
#define VIZ_4D_MATH_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "../debug.h"

namespace viz {
namespace math {
enum Axis { X, Y, Z, W };

enum Plane { XY, XZ, XW, YX, YZ, YW, ZX, ZY, ZW, WX, WY, WZ };

std::pair<Axis, Axis> plane_to_axis(Plane p) {
  switch (p) {
    case XY:
    case YX:
      return {X, Y};
    case XZ:
    case ZX:
      return {X, Z};
    case XW:
    case WX:
      return {X, W};
    case ZY:
    case YZ:
      return {Y, Z};
    case WY:
    case YW:
      return {Y, W};
    case WZ:
    case ZW:
      return {Z, W};
  }
  std::cerr << "critical error in 4d_math, incorrect plane";
  return {X, X};
}
/// @brief
/// @param a Turning axis 1.
/// @param b Turning axis 2.
/// @param angle Angle in radians.
/// @return Rotation matrix, which rotates the plane p.
glm::mat4 rotate(Axis a, Axis b, float angle) {
  std::vector<bool> turns(4, false);
  turns[a] = true;
  turns[b] = true;

  glm::mat4 res(0);

  bool first_column = true;
  for (int i = 0; i < 4; ++i) {
    bool first_row = true;
    for (int j = 0; j < 4; ++j) {
      if (turns[i]) {
        if (first_column) {
          if (turns[j]) {
            if (first_row) {
              res[i][j] = glm::cos(angle);
              first_row = false;
            } else {
              res[i][j] = glm::sin(angle);
            }
          }
          first_column = false;
        } else {
          if (turns[j]) {
            if (first_row) {
              std::cout << "-sin" << std::endl;
              res[i][j] = -1 * glm::sin(angle);
              first_row = false;
            } else {
              res[i][j] = glm::cos(angle);
            }
          }
        }
      } else {
        if (i == j) {
          res[i][j] = 1;
        }
      }
    }
  }

  return res;
}

/// @brief
/// @param p Turning plane.
/// @param angle Angle in radians.
/// @return Rotation matrix, which rotates the plane p.
inline glm::mat4 rotate(Plane p, float angle) {
  glm::mat4 res =
      rotate(plane_to_axis(p).first, plane_to_axis(p).second, angle);

  std::cerr << "---------rad " << angle << "--------" << std::endl;
  std::cerr << res << std::endl;
  std::cerr << "-------------------------------";
  return res;
}
}  // namespace math
}  // namespace viz

#endif  // VIZ_4D_MATH_H