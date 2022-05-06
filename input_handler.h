#ifndef VIZ_INPUT_HANDLER
#define VIZ_INPUT_HANDLER

#include <GLFW/glfw3.h>
#include "camera_manager.h"

class InputHandler {
 public:
  InputHandler(GLFWwindow* window, CameraManager* camera_manager);

 private:
  GLFWwindow* window;
  CameraManager* camera_manager;
};

#endif  // VIZ_INPUT_HANDLER