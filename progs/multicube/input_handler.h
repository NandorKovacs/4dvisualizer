#ifndef VIZ_INPUT_HANDLER
#define VIZ_INPUT_HANDLER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "../../lib/camera_manager.h"
#include "../../lib/hyperplane_manager.h"
#include "../../lib/key_manager.h"
#include "renderer.h"

namespace viz {

class InputHandler {
 public:
  InputHandler(GLFWwindow* window, CameraManager& camera_manager, HyperplaneManager& hyperplane_manager, Renderer& renderer);

  void on_key_action(GLFWwindow* window, int key, int scancode, int action,
                     int mods);

  void on_mouse_action(GLFWwindow* window, double x, double y);

 private:
  glm::vec2 mouse_pos;
  bool initialized = false;
  GLFWwindow* window;
  CameraManager& camera_manager;
  HyperplaneManager& hyperplane_manager;
  Renderer& renderer;

  std::vector<key_manager::KeyManager> key_managers;
};

}  // namespace viz
#endif  // VIZ_INPUT_HANDLER