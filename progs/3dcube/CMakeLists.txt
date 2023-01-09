add_executable(3dcube
  cube.cc
  input_handler.cc
  renderer.cc
)

target_link_libraries(3dcube
  4dvisualizer_lib
  viz_3dcube_glsl
  glew_s
  glfw
  glm::glm
)

cmrc_add_resource_library(viz_3dcube_glsl
  vshader.glsl
  fshader.glsl
)