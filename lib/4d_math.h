#ifndef VIZ_4D_MATH_H
#define VIZ_4D_MATH_H

#include <glm/glm.hpp>
#include <vector>

enum Axis { X, Y, Z, W };

enum Plane { XY, XZ, XW, YX, YZ, YW, ZX, ZY, ZW, WX, WY, WZ };

std::pair<Axis, Axis> plane_to_axis(Plane p);

/// @brief
/// @param a Turning axis 1.
/// @param b Turning axis 2.
/// @param angle Angle in radians.
/// @return Rotation matrix, which rotates the plane p.
glm::mat4 rotate(Axis a, Axis b, int angle);

/// @brief
/// @param p Turning plane.
/// @param angle Angle in radians.
/// @return Rotation matrix, which rotates the plane p.
inline glm::mat4 rotate(Plane p, int angle);



#endif  // VIZ_4D_MATH_H