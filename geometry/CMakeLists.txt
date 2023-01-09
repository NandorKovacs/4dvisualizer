

add_library(geometry
  intersect.cc
  4d_math.cc
  geometry.cc
)

target_link_libraries(geometry
PUBLIC
  glm::glm
)

add_executable(intersect_test
  intersect_test.cc
)

target_link_libraries(intersect_test
  geometry
  GTest::gtest_main
  glm::glm
)

gtest_discover_tests(intersect_test)