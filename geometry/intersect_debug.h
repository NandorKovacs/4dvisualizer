#ifndef VIZ_GEOMETRY_INTERSECT_DEBUG_H
#define VIZ_GEOMETRY_INTERSECT_DEBUG_H

/// @file
/// Debug routines for the intesection logic.

#include <ostream>

#include "intersect.h"

namespace viz {
namespace intersect {

/// @brief Prints a human readable form of a triangle.
std::ostream& operator<<(std::ostream& os, Triangle const& t) {
  os << "Triangle{";
  for (int i = 0; i < t.pts.size(); ++i) {
    const glm::vec3& v = t.pts[i];
    os << v.x << ", " << v.y << ", " << v.z;
    if (i != t.pts.size() - 1) {
      os << "; ";
    }
  }
  os << "}";
  return os;
}

}  // namespace intersect
}  // namespace viz

#endif  // VIZ_GEOMETRY_DEBUG_H
