#include "intersect.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "intersect_debug.h"

namespace viz {
namespace intersect {
namespace {

TEST(IntersectTest, IntersectWithDefaultPlane) {
  Intersector intersector;
  Hyperplane plane;
  plane.pos = glm::vec4(0, 0, 0, 0);
  plane.normal() = glm::vec4(0, 0, 0, 1);

  std::vector<Triangle> triangles;
  intersector.intersect(
      [&](Triangle const& t) {
        triangles.push_back(t);
      },
      plane);

  EXPECT_EQ(24, triangles.size());

  const std::vector<std::string> expected_lines = {
      "Triangle{-1, -1, -1, 0; -1, 1, -1, 0; -1, -1, 1, 0}",
      "Triangle{-1, -1, -1, 0; 1, -1, -1, 0; -1, -1, 1, 0}",
      "Triangle{-1, -1, -1, 0; 1, -1, -1, 0; -1, 1, -1, 0}",
      "Triangle{-1, -1, 1, 0; -1, 1, 1, 0; -1, -1, -1, 0}",
      "Triangle{-1, -1, 1, 0; 1, -1, 1, 0; -1, -1, -1, 0}",
      "Triangle{-1, -1, 1, 0; 1, -1, 1, 0; -1, 1, 1, 0}",
      "Triangle{-1, 1, -1, 0; -1, -1, -1, 0; -1, 1, 1, 0}",
      "Triangle{-1, 1, -1, 0; 1, 1, -1, 0; -1, -1, -1, 0}",
      "Triangle{-1, 1, -1, 0; 1, 1, -1, 0; -1, 1, 1, 0}",
      "Triangle{-1, 1, 1, 0; -1, -1, 1, 0; -1, 1, -1, 0}",
      "Triangle{-1, 1, 1, 0; 1, 1, 1, 0; -1, -1, 1, 0}",
      "Triangle{-1, 1, 1, 0; 1, 1, 1, 0; -1, 1, -1, 0}",
      "Triangle{1, -1, -1, 0; -1, -1, -1, 0; 1, -1, 1, 0}",
      "Triangle{1, -1, -1, 0; -1, -1, -1, 0; 1, 1, -1, 0}",
      "Triangle{1, -1, -1, 0; 1, 1, -1, 0; 1, -1, 1, 0}",
      "Triangle{1, -1, 1, 0; -1, -1, 1, 0; 1, -1, -1, 0}",
      "Triangle{1, -1, 1, 0; -1, -1, 1, 0; 1, 1, 1, 0}",
      "Triangle{1, -1, 1, 0; 1, 1, 1, 0; 1, -1, -1, 0}",
      "Triangle{1, 1, -1, 0; -1, 1, -1, 0; 1, -1, -1, 0}",
      "Triangle{1, 1, -1, 0; -1, 1, -1, 0; 1, 1, 1, 0}",
      "Triangle{1, 1, -1, 0; 1, -1, -1, 0; 1, 1, 1, 0}",
      "Triangle{1, 1, 1, 0; -1, 1, 1, 0; 1, -1, 1, 0}",
      "Triangle{1, 1, 1, 0; -1, 1, 1, 0; 1, 1, -1, 0}",
      "Triangle{1, 1, 1, 0; 1, -1, 1, 0; 1, 1, -1, 0}",
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
    EXPECT_EQ(got_lines[i], expected_lines[i]) << "Triangles differ at index " << i;
  }
}

}  // namespace
}  // namespace intersect
}  // namespace viz
