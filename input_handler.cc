#include "input_handler.h"

#include <GL/glew.h>

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
static void mouse_event_callback(GLFWwindow* window, double x, double y) {
  auto it = handlers.find(window);
  if (it == handlers.end()) {
    return;
  }
  it->second->on_mouse_action(window, x, y);
}
}
void InputHandler::on_key_action(GLFWwindow* window, int key, int scancode,
                                 int action, int mods) {
  glm::vec3 move_direction = camera_manager->get_move_directions();
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_LEFT_CONTROL:
        // DLOG << "ctrl press" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(move_direction.x, 1, move_direction.z));
        break;
      case GLFW_KEY_LEFT_SHIFT:
      case GLFW_KEY_SPACE:
        // DLOG << "shift press" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(move_direction.x, -1, move_direction.z));
        break;
      case GLFW_KEY_A:
        // DLOG << "A press" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(1, move_direction.y, move_direction.z));
        break;
      case GLFW_KEY_D:
        // DLOG << "D press" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(-1, move_direction.y, move_direction.z));
        break;
      case GLFW_KEY_W:
        // DLOG << "W press" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(move_direction.x, move_direction.y, 1));
        break;
      case GLFW_KEY_S:
        // DLOG << "S press" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(move_direction.x, move_direction.y, -1));
        break;
      default:
        break;
    }
  }
  if (action == GLFW_RELEASE) {
    switch (key) {
      case GLFW_KEY_LEFT_SHIFT:
      case GLFW_KEY_SPACE:
        // DLOG << "shift release" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(move_direction.x, 0, move_direction.z));
        break;
      case GLFW_KEY_LEFT_CONTROL:
        // DLOG << "ctrl release" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(move_direction.x, 0, move_direction.z));
        break;
      case GLFW_KEY_A:
        // DLOG << "A release" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(0, move_direction.y, move_direction.z));
        break;
      case GLFW_KEY_D:
        // DLOG << "D release" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(0, move_direction.y, move_direction.z));
        break;
      case GLFW_KEY_W:
        // DLOG << "W release" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(move_direction.x, move_direction.y, 0));
        break;
      case GLFW_KEY_S:
        // DLOG << "S release" << std::endl;
        camera_manager->set_move_direction(
            glm::vec3(move_direction.x, move_direction.y, 0));
        break;
      default:
        break;
    }
  }
}

void InputHandler::on_mouse_action(GLFWwindow* window, double x, double y) {
  if (!initialized) {
    mouse_pos = glm::vec2(x, y);
    initialized = true;
    return;
  }
  glm::vec2 pos = glm::vec2(x, y);
  glm::vec2 movement = mouse_pos - pos;
  mouse_pos = pos;

  camera_manager->rotate(movement);
}

InputHandler::InputHandler(GLFWwindow* window, CameraManager* camera_manager)
    : window{window}, camera_manager{camera_manager} {
  glfwSetKeyCallback(window, key_event_callback);
  CHECK_GLFW("set_key_callback");
  glfwSetCursorPosCallback(window, mouse_event_callback);
  CHECK_GLFW("set_mouse_callback");
  handlers[window] = this;
}