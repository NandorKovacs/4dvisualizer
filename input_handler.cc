#include "input_handler.h"

#include <GLFW/glfw3.h>

#include <unordered_map>

#include "lib/errors.h"

// The GLFW library calls our callbacks with a window pointer. We
// use this map to find out which handler belongs to which window.
static std::unordered_map<GLFWwindow*, InputHandler*> handlers;

extern "C" {
static void key_event_callback(GLFWwindow* window, int key, int scancode,
                               int action, int mods) {
  auto it = handlers.find(window);
  if (it == handlers.end()) {
    return;
  }
  it->second->on_key_action(window, key, scancode, action, mods);
}
}

void InputHandler::on_key_action(GLFWwindow* window, int key, int scancode,
                                 int action, int mods) {
  
  
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_LEFT_SHIFT:
        //camera_manager->set_move_speed(glm::cross(camera_manager->get_angle(), glm));
        break;
      case GLFW_KEY_LEFT_CONTROL:
        /* code */
        break;
      case GLFW_KEY_W:
        /* code */
        break;
      case GLFW_KEY_S:
        /* code */
        break;
      case GLFW_KEY_A:
        /* code */
        break;
      case GLFW_KEY_D:
        /* code */
        break;
      default:
        break;
    }
  }
}

InputHandler::InputHandler(GLFWwindow* window, CameraManager* camera_manager)
    : window{window}, camera_manager{camera_manager} {
  glfwSetKeyCallback(window, key_event_callback);
  CHECK_GLFW("set_key_callback");
  handlers[window] = this;
}