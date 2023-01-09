#ifndef VIZ_GEOMETRY_H
#define VIZ_GEOMETRY_H

#include <array>
#include <glm/glm.hpp>

namespace viz {

/// @brief A 3 dimensional hyperplane in a 4 dimensional space.
///
/// Represents a 3 dimensional hyperplane in a 4 dimensional space,
/// given with a point on it, and a 4 dimensional normal vector.
///
/// It also maintains a coordinate system within the 3 dimensional
/// hyperplane. Just like the other parameters, these corrdinate
/// vectors are also represented in the outer 4 dimensional space.
struct Hyperplane {
  Hyperplane() = delete;

  /// @brief Constructs a hyperplane.
  /// @param pos The origin of the inner 3 dimensional coordinate system.
  /// @param coord_system A 4x4 matrix, of which the first three column
  ///     vectors are the coordinate system within the 3 dimensional
  ///     hyperplane, and the 4th column vector is the normal vector of the
  ///     hyperplane. The provided column vectors must be orthogonal unit
  ///     vectors.
  Hyperplane(glm::vec4 pos, glm::mat4 coord_system)
      : pos{pos}, coord_system{coord_system} {}

  /// @return The normal vector of the hyperplane.
  inline glm::vec4 const& normal() const { return coord_system[3]; }
  inline glm::vec4& normal() { return coord_system[3]; }

  /// @return The x unit vector of the 3 dimensional coordinate system
  ///     within the hyperplane.
  inline glm::vec4 const& unit_x() const { return coord_system[0]; }
  inline glm::vec4& unit_x() { return coord_system[0]; }

  /// @return The y unit vector of the 3 dimensional coordinate system
  ///     within the hyperplane.
  inline glm::vec4 const& unit_y() const { return coord_system[1]; }
  inline glm::vec4& unit_y() { return coord_system[1]; }

  /// @return The z unit vector of the 3 dimensional coordinate system
  ///     within the hyperplane.
  inline glm::vec4 const& unit_z() const { return coord_system[2]; }
  inline glm::vec4& unit_z() { return coord_system[2]; }

  /// The origin of then 3 dimensional coordinate system within the
  /// hyperplane.
  glm::vec4 pos;

  /// A 4x4 matrix, of which the first three column
  /// vectors are the coordinate system within the 3 dimensional
  /// hyperplane, and the 4th column vector is the normal vector of the
  /// hyperplane. The provided column vectors must be orthogonal unit
  /// vectors.
  glm::mat4 coord_system;

  glm::vec4 to_abs(glm::vec3 pt) const;
  glm::vec3 to_rel(glm::vec4 pt) const;
};

}  // namespace viz

#endif  // VIZ_GEOMETRY_H