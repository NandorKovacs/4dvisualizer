

add_library(geometry
  intersect.cc
)

add_executable(intersect_test
  intersect_test.cc
)
target_link_libraries(intersect_test
  geometry
  GTest::gtest_main
)

gtest_discover_tests(intersect_test)