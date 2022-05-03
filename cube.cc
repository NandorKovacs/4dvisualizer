#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <exception>
#include <iostream>

#include "lib/errors.h"
#include "renderer.h"
#include "window_size.h"

constexpr double frame_wait_time = 0.01;

int main(int argc, char** argv) {
  GLFWwindow* window;

  if (!glfwInit()) {
    CHECK_GLFW("glfwInit");
    return 1;
  };
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(1024, 800, argv[0], NULL, NULL);
  CHECK_GLFW("glfwCreateWindow");

  if (!window) {
    std::cerr << "Error: window not created." << std::endl;
    std::terminate();
  }

  glfwMakeContextCurrent(window);
  CHECK_GLFW("glfwMakeContextCurrent");

  glewExperimental = GL_TRUE;
  GLenum glew_err = glewInit();
  if (GLEW_OK != glew_err) {
    std::cerr << "Glew init error: " << glewGetErrorString(glew_err)
              << std::endl;
    std::terminate();
  }

  WindowSizeTracker window_size{window};
  Renderer renderer;
  renderer.init();

  while (!glfwWindowShouldClose(window)) {
    renderer.render(window, glfwGetTime());
    glfwSwapBuffers(window);
  }
  glfwDestroyWindow(window);
  return 0;
}
