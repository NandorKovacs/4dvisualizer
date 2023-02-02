add_executable(keycode_searcher
  main.cc
  key_input.cc
)

target_link_libraries(keycode_searcher
PUBLIC
  glew_s
  glfw
)