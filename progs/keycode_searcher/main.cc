#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <exception>
#include <iostream>

#include "key_input.h"

constexpr double frame_wait_time = 0.01;

int main(int argc, char** argv) {
  GLFWwindow* window;

  if (!glfwInit()) {
    return 1;
  };
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(1024, 800, "4dviz", NULL, NULL);

  if (!window) {
    std::cerr << "Error: window not created." << std::endl;
    std::terminate();
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  GLenum glew_err = glewInit();
  if (GLEW_OK != glew_err) {
    std::cerr << "Glew init error: " << glewGetErrorString(glew_err)
              << std::endl;
    std::terminate();
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }

  viz::utils::InputHandler handler{window};

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwDestroyWindow(window);
  return 0;
}
