#ifndef VIZ_INPUT_HANDLER
#define VIZ_INPUT_HANDLER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera_manager.h"

class InputHandler {
 public:
  InputHandler(GLFWwindow* window, CameraManager* camera_manager);

  void on_key_action(GLFWwindow* window, int key, int scancode, int action,
                     int mods);

  void on_mouse_action(GLFWwindow* window, double x, double y);

 private:
  glm::vec2 mouse_pos;
  bool initialized = false;
  GLFWwindow* window;
  CameraManager* camera_manager;
};

#endif  // VIZ_INPUT_HANDLER