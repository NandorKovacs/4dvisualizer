/// @file
/// Tests for the intersection logic.

#include "intersect.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <vector>

#include "intersect_debug.h"

namespace viz {
namespace intersect {
namespace {

/// @brief Smoke test, intersecting the canonical unit cube with the
///      plane in the origin, orthogonal to the 4th unit vector.
TEST(IntersectTest, IntersectWithDefaultPlane) {
  Intersector intersector;
  Hyperplane plane{glm::vec4(0, 0, 0, 0),
                   glm::mat4{glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
                             glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1)}};

  std::vector<Triangle> triangles;
  intersector.intersect(
      [&](Triangle const& t, glm::vec3 const& normal) {
        triangles.push_back(t);
        // std::cerr << t << std::endl;
      },
      plane);

  const std::vector<std::string> expected_lines = {
      "Triangle{-1, -1, -1; -1, 1, -1; -1, 1, 1}",
      "Triangle{-1, -1, -1; 1, -1, -1; 1, -1, 1}",
      "Triangle{-1, -1, -1; 1, -1, -1; 1, 1, -1}",
      "Triangle{-1, -1, 1; 1, -1, 1; 1, 1, 1}",
      "Triangle{-1, 1, -1; 1, 1, -1; 1, 1, 1}",
      "Triangle{-1, 1, 1; -1, -1, 1; -1, -1, -1}",
      "Triangle{1, -1, -1; 1, 1, -1; 1, 1, 1}",
      "Triangle{1, -1, 1; -1, -1, 1; -1, -1, -1}",
      "Triangle{1, 1, -1; -1, 1, -1; -1, -1, -1}",
      "Triangle{1, 1, 1; -1, 1, 1; -1, -1, 1}",
      "Triangle{1, 1, 1; -1, 1, 1; -1, 1, -1}",
      "Triangle{1, 1, 1; 1, -1, 1; 1, -1, -1}",
  };

  std::vector<std::string> got_lines;
  for (Triangle const& tr : triangles) {
    std::ostringstream oss;
    oss << tr;
    got_lines.push_back(oss.str());
  }

  std::sort(got_lines.begin(), got_lines.end());

  EXPECT_EQ(got_lines.size(), expected_lines.size());

  for (int i = 0; i < got_lines.size(); ++i) {
    EXPECT_EQ(got_lines[i], expected_lines[i])
        << "Triangles differ at index " << i;
  }
}

}  // namespace
}  // namespace intersect
}  // namespace viz
