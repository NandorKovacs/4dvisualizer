add_executable(4dcube
  cube.cc
  input_handler.cc
  renderer.cc
)

target_link_libraries(4dcube
PUBLIC
  4dvisualizer_lib
  geometry
  viz_4dcube_glsl
  glew_s
  glfw
  glm::glm
)

cmrc_add_resource_library(viz_4dcube_glsl
  wireframe_vshader.glsl
  wireframe_fshader.glsl
  light_vshader.glsl
  light_fshader.glsl
)