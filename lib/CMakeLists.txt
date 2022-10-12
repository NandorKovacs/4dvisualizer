add_library(4dvisualizer_lib
  errors.cc
  camera_manager.cc
  shader_loader.cc
  window_size.cc
  hyperplane_manager.cc
  key_manager.cc
)

target_link_libraries(4dvisualizer_lib
  ${GL_LIB}
)