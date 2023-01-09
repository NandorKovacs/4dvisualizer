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
  ${GL_LIB}
)

cmrc_add_resource_library(viz_multicube_glsl
  wireframe_vshader.glsl
  wireframe_fshader.glsl
  light_vshader.glsl
  light_fshader.glsl
)
