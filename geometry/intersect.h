#ifndef VIZ_INTERSECT_H
#define VIZ_INTERSECT_H

#include <array>
#include <cstdint>
#include <glm/glm.hpp>

namespace viz {
namespace intersect {

struct PointOnEdge {
  glm::vec4 coords;
  std::uint32_t faces = 0;
};

struct Hyperplane {
  glm::vec4 normal;
  glm::vec4 pos;
};

/**
 * @brief Intersects the -1 +1 4d cube with an arbitrary hyperplane.
 *
 * @param res Points of intersection.
 * @param plane
 * @return int The number of valid items in res.
 */
int intersect(std::array<PointOnEdge, 32>& res, Hyperplane const& plane);

}  // namespace intersect
}  // namespace viz
#endif  // VIZ_INTERSECT_H