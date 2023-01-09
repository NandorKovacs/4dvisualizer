#include "input_handler.h"

#include <GL/glew.h>

#include <functional>
#include <unordered_map>

#include "../../lib/scan_codes.h"
#include "../../lib/errors.h"
#include "../../lib/key_manager.h"

using namespace std::placeholders;
namespace viz {
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
  if (scancode == VIZ_SC_Q) {
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
  }
  for (key_manager::KeyManager& km : key_managers) {
    km.on_action(scancode, action);
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

  camera_manager.rotate(movement);
}

InputHandler::InputHandler(GLFWwindow* window, CameraManager& camera_manager)
    : window{window}, camera_manager{camera_manager} {
  glfwSetKeyCallback(window, key_event_callback);
  CHECK_GLFW("set_key_callback");
  glfwSetCursorPosCallback(window, mouse_event_callback);
  CHECK_GLFW("set_mouse_callback");
  handlers[window] = this;

  auto set_move_X = std::bind(&CameraManager::set_move_X, &camera_manager, _1);
  auto set_move_Y = std::bind(&CameraManager::set_move_Y, &camera_manager, _1);
  auto set_move_Z = std::bind(&CameraManager::set_move_Z, &camera_manager, _1);

  key_managers.push_back(key_manager::KeyManager{
      {VIZ_SC_A},
      {VIZ_SC_D},
      set_move_X,
      1.0f,
  });
  key_managers.push_back(key_manager::KeyManager{
      {VIZ_SC_L_CTRL},
      {VIZ_SC_L_SHIFT, VIZ_SC_SPACE},
      set_move_Y,
      1.0f,
  });
  key_managers.push_back(key_manager::KeyManager{
      {VIZ_SC_W},
      {VIZ_SC_S},
      set_move_Z,
      1.0f,
  });
}

}  // namespace viz