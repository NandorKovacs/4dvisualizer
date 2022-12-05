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
  ${GL_LIB}
)

cmrc_add_resource_library(viz_4dcube_glsl
  wireframe_vshader.glsl
  wireframe_fshader.glsl
  vshader.glsl
  fshader.glsl
)