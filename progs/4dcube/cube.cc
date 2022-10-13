#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <exception>
#include <iostream>

#include "../../lib/camera_manager.h"
#include "../../lib/errors.h"
#include "../../lib/window_size.h"
#include "input_handler.h"
#include "renderer.h"

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
  window = glfwCreateWindow(1024, 800, "4dviz", NULL, NULL);
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

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  CHECK_GLFW("glfwSetInputMode");
  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    CHECK_GLFW("glfwSetInputMode2");
  }

  WindowSizeTracker window_size{window};
  CameraManager camera_manager;
  viz::HyperplaneManager hyperplane_manager;

  viz::InputHandler input_handler(window, camera_manager, hyperplane_manager);
  viz::Renderer renderer{window_size.get(), camera_manager, hyperplane_manager};

  double prev_time = 0;
  while (!glfwWindowShouldClose(window)) {
    if (window_size.should_update()) {
      renderer.set_size(window_size.get());
    }
    renderer.render();
    glfwPollEvents();
    CHECK_GLFW("poll events");
    double time = glfwGetTime();
    CHECK_GLFW("get time");
    double time_diff = time - prev_time;
    prev_time = time;

    camera_manager.tick(time_diff);
    hyperplane_manager.tick(time_diff);

    glfwSwapBuffers(window);
    CHECK_GLFW("bufferswap");
  }
  glfwDestroyWindow(window);
  return 0;
}
