#ifndef VIZ_INPUT_HANDLER
#define VIZ_INPUT_HANDLER

#include <GLFW/glfw3.h>
#include "camera_manager.h"

class InputHandler {
 public:
  InputHandler(GLFWwindow* window, CameraManager* camera_manager);

  void on_key_action(GLFWwindow* window, int key, int scancode,
                               int action, int mods);
 private:
  GLFWwindow* window;
  CameraManager* camera_manager;
};

#endif  // VIZ_INPUT_HANDLER