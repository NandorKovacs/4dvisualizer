#include "4d_math.h"

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
}

glm::mat4 rotate(Axis a, Axis b, int angle) {
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
            } else {
              res[i][j] = glm::sin(angle);
            }
          }
        } else {
          if (turns[j]) {
            if (first_row) {
              res[i][j] = -glm::sin(angle);
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

inline glm::mat4 rotate(Plane p, int angle) {
  return rotate(plane_to_axis(p).first, plane_to_axis(p).second, angle);
}