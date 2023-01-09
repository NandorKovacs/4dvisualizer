add_library(4dvisualizer_lib
  errors.cc
  camera_manager.cc
  shader_loader.cc
  window_size.cc
  hyperplane_manager.cc
  key_manager.cc
  world_loader.cc
)

target_link_libraries(4dvisualizer_lib
PUBLIC
  glew_s
  yaml-cpp
  glm::glm
)

