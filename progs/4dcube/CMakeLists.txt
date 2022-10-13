add_executable(4dcube
  cube.cc
  input_handler.cc
  renderer.cc
)

target_link_libraries(4dcube
  4dvisualizer_lib
  geometry
  ${GL_LIB}
)