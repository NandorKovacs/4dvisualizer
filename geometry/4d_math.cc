#include "4d_math.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../debug.h"

namespace viz {
namespace math {

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
  throw std::runtime_error{"error in 4d_math, incorrect plane"};
}

static const std::array<std::string, 12> plane_names{
    "XY", "XZ", "XW", "YX", "YZ", "YW", "ZX", "ZY", "ZW", "WX", "WY", "WZ"};

Plane planeFromString(std::string s) {
  for (int i = 0; i < plane_names.size(); ++i) {
    if (plane_names[i] == s) {
      return static_cast<Plane>(i);
    }
  }
  std::ostringstream err;
  err << "unknown plane: " << s;
  throw std::runtime_error(err.str());
}

glm::mat4 rotations(std::vector<Rotation> const& rotations) {
  glm::mat4 m{1.0f};
  for (auto const& rotation : rotations) {
    m = rotate(rotation.plane, rotation.angle) * m;
  }
  return m;
}

glm::mat4 rotate(Axis a, Axis b, float angle) {
  std::vector<bool> turns(4, false);
  turns[a] = true;
  turns[b] = true;

  glm::mat4 res(0.0f);

  bool first_column = true;
  for (int i = 0; i < 4; ++i) {
    bool first_row = true;
    for (int j = 0; j < 4; ++j) {
      if (turns[i] && turns[j]) {
        if (first_column) {
          if (first_row) {
            res[i][j] = glm::cos(angle);
            first_row = false;
          } else {
            res[i][j] = glm::sin(angle);
            first_column = false;
          }           
        } else {
          if (first_row) {
            res[i][j] = -1 * glm::sin(angle);
            first_row = false;
          } else {
            res[i][j] = glm::cos(angle);
          }
        }
      } else {
        if (i == j) {
          res[i][j] = 1;
          continue;
        }
      }
    }
  }

  return res;
}

glm::mat4 rotate(Plane p, float angle) {
  glm::mat4 res =
      rotate(plane_to_axis(p).first, plane_to_axis(p).second, angle);

  return res;
}

Transformation::Transformation(glm::mat4 rotation, float scaling,
                               glm::vec4 translate)
    : rotation{rotation},
      inverse_rotation{glm::inverse(rotation)},
      scaling{scaling},
      translate{translate} {}

glm::vec4 Transformation::forward(glm::vec4 const& v) const {
  glm::vec4 res = rotation * v * scaling + translate;
  return res;
}

glm::vec4 Transformation::reverse(glm::vec4 const& v) const {
  glm::vec4 res = inverse_rotation * (v - translate) / scaling;
  return res;
}

glm::vec4 Transformation::forward_rotate(glm::vec4 const& v) const {
  glm::vec4 res = rotation * v;
  return res;
}

// TODO: this is a different reverse: does not do shift, for unit vectors.
// Refactor!
glm::vec4 Transformation::reverse_rotate(glm::vec4 const& v) const {
  glm::vec4 res = inverse_rotation * v;
  return res;
}

glm::mat4 Transformation::reverse_rotate_matrix_columns(
    glm::mat4 const& m) const {
  return glm::mat4{reverse_rotate(m[0]), reverse_rotate(m[1]),
                   reverse_rotate(m[2]), reverse_rotate(m[3])};
}

}  // namespace math
}  // namespace viz
