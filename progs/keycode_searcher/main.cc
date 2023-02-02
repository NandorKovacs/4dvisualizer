#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <exception>
#include <iostream>

#include "key_input.h"

int main(int argc, char** argv) {
  GLFWwindow* window;

  if (!glfwInit()) {
    return 1;
  };
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(50, 50, "4dviz", NULL, NULL);

  if (!window) {
    std::cerr << "Error: window not created." << std::endl;
    std::terminate();
  }

  glfwMakeContextCurrent(window);

  viz::utils::InputHandler handler{window};

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwDestroyWindow(window);
  return 0;
}
