#ifndef VIZ_4D_MATH_H
#define VIZ_4D_MATH_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <string>

#include "../debug.h"

namespace viz {
namespace math {

enum Axis { X, Y, Z, W };

enum Plane { XY, XZ, XW, YX, YZ, YW, ZX, ZY, ZW, WX, WY, WZ };

Plane planeFromString(std::string s);

struct Rotation {
  Plane plane;
  float angle;
};

glm::mat4 rotations(std::vector<Rotation> const& rotations);

std::pair<Axis, Axis> plane_to_axis(Plane p);

/// @brief
/// @param a Turning axis 1.
/// @param b Turning axis 2.
/// @param angle Angle in radians.
/// @return Rotation matrix, which rotates the plane p.
glm::mat4 rotate(Axis a, Axis b, float angle);

/// @brief
/// @param p Turning plane.
/// @param angle Angle in radians.
/// @return Rotation matrix, which rotates the plane p.
glm::mat4 rotate(Plane p, float angle);


struct Transformation {
  Transformation(glm::mat4 rotation, float scaling, glm::vec4 translate);

  glm::vec4 forward(glm::vec4 const& v) const;
  glm::vec4 reverse(glm::vec4 const& v) const;

  glm::vec4 forward_rotate(glm::vec4 const& v) const;
  glm::vec4 reverse_rotate(glm::vec4 const& v) const;

  glm::mat4 reverse_rotate_matrix_columns(glm::mat4 const& m) const;

  private:
    glm::mat4 rotation;
    glm::mat4 inverse_rotation;
    float scaling;
    glm::vec4 translate;
};


}  // namespace math
}  // namespace viz

#endif  // VIZ_4D_MATH_H