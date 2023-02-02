add_executable(multicube
  cube.cc
  input_handler.cc
  renderer.cc
)

target_link_libraries(multicube
PUBLIC
  4dvisualizer_lib
  geometry
  viz_multicube_glsl
  glew_s
  glfw
  glm::glm
)

cmrc_add_resource_library(viz_multicube_glsl
  wireframe_vshader.glsl
  wireframe_fshader.glsl
  light_vshader.glsl
  light_fshader.glsl
  color_vshader.glsl
  color_fshader.glsl
)
