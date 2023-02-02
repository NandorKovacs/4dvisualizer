cmake_minimum_required(VERSION 3.22.1)

project(4dvisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Dependencies

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  cmrc
  GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
  GIT_TAG 2.0.1
)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.7.0
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)

FetchContent_Declare(
  glew
  SOURCE_SUBDIR build/cmake
  URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz
)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)

# ------------------------------
FetchContent_MakeAvailable(googletest cmrc yaml-cpp glm glew glfw)

enable_testing()
include(GoogleTest)

if (WIN32)
  include_directories(
    "${glew_SOURCE_DIR}/include"
    "${GLFW_SOURCE_DIR}/include"
  )
endif()

add_subdirectory(geometry)
add_subdirectory(lib)
add_subdirectory(progs/3dcube)
add_subdirectory(progs/4dcube)
add_subdirectory(progs/multicube)
add_subdirectory(progs/keycode_searcher)
