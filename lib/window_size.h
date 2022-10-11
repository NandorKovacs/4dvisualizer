#ifndef VIZ_WINDOW_SIZE_H
#define VIZ_WINDOW_SIZE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

#include "window_size.h"

extern "C" {

void window_size_callback(GLFWwindow* window, int width, int height);
}

class WindowSize {
 public:
  virtual glm::ivec2 get() const = 0;
};

class WindowSizeTracker : public WindowSize {
 public:
  WindowSizeTracker(GLFWwindow* window);
  glm::ivec2 get() const override;
  bool should_update();

 private:
  friend void ::window_size_callback(GLFWwindow* window, int width, int height);

  void on_size_changed(int width, int height);
  GLFWwindow* window;
  glm::ivec2 size;
  bool did_change;
};

#endif  // VIZ_WINDOW_SIZE
