add_executable(3dcube
  cube.cc
  input_handler.cc
  renderer.cc
)

target_link_libraries(3dcube
  4dvisualizer_lib
  ${GL_LIB}
)