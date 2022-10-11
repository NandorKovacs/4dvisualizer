#include "window_size.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <unordered_map>

#include "errors.h"

static std::unordered_map<GLFWwindow*, WindowSizeTracker*> trackers;

extern "C" {

void window_size_callback(GLFWwindow* window, int width, int height) {
  auto it = trackers.find(window);
  if (it == trackers.end()) {
    return;
  }
  it->second->on_size_changed(width, height);
}

}  // extern "C"

WindowSizeTracker::WindowSizeTracker(GLFWwindow* window) : window(window) {
  glfwGetWindowSize(window, &size.x, &size.y);
  CHECK_GLFW("glfwGetWindowSize");

  glfwSetWindowSizeCallback(window, window_size_callback);
  CHECK_GLFW("glfwSetWindowSizeCallback");

  trackers[window] = this;
  did_change = false;
}

void WindowSizeTracker::on_size_changed(int width, int height) {
  size.x = width;
  size.y = height;
  did_change = true;
  glViewport(0, 0, width, height);
}

glm::ivec2 WindowSizeTracker::get() const { return size; }

bool WindowSizeTracker::should_update() {
  bool old_did_change = did_change;
  did_change = false;
  return old_did_change;
}
