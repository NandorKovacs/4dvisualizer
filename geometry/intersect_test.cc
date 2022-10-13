#include "intersect.h"

#include <gtest/gtest.h>
#include <vector>

namespace viz {
namespace intersect {
namespace {



TEST(IntersectTest, IntersectWithDefaultPlane) {
  Intersector intersector;
  Hyperplane plane;
  plane.pos = glm::vec4(0,0,0,0);
  plane.normal() = glm::vec4(0,0,0,1);
  
  std::vector<Triangle> triangles;
  intersector.intersect([&](Triangle const& t){
    triangles.push_back(t);
  },plane);
  
  
  EXPECT_EQ(24, triangles.size());
}

}  // namespace
}  // namespace intersect
}  // namespace viz
