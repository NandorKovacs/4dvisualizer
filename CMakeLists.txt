cmake_minimum_required(VERSION 3.22.1)

project(4dvisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# GoogleTest --------

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

include(GoogleTest)

# CMRC --------------

FetchContent_Declare(
  cmrc
  GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
  GIT_TAG 2.0.1
)
FetchContent_MakeAvailable(cmrc)

# ------------------------------

set(GL_LIB 
  -lglfw
  -lGLEW
  -lGL
)

add_subdirectory(geometry)
add_subdirectory(lib)
add_subdirectory(progs/3dcube)
add_subdirectory(progs/4dcube)
