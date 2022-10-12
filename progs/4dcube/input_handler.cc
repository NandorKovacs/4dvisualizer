#include "input_handler.h"

#include <GL/glew.h>

#include <functional>
#include <unordered_map>

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

InputHandler::InputHandler(GLFWwindow* window, CameraManager& camera_manager,
                           HyperplaneManager& hyperplane_manager)
    : window{window},
      camera_manager{camera_manager},
      hyperplane_manager{hyperplane_manager} {
  glfwSetKeyCallback(window, key_event_callback);
  CHECK_GLFW("set_key_callback");
  glfwSetCursorPosCallback(window, mouse_event_callback);
  CHECK_GLFW("set_mouse_callback");
  handlers[window] = this;

  key_managers.push_back(key_manager::KeyManager{
      {/*A*/ 38},
      {/*D*/ 40},
      std::bind(&CameraManager::set_move_X, &camera_manager, _1),
      1.0f,
  });
  key_managers.push_back(key_manager::KeyManager{
      {/*L_CTRL*/ 37},
      {/*L_SHIFT*/ 50, /*SPACE*/ 65},
      std::bind(&CameraManager::set_move_Y, &camera_manager, _1),
      1.0f,
  });
  key_managers.push_back(key_manager::KeyManager{
      {/*W*/ 25},
      {/*S*/ 39},
      std::bind(&CameraManager::set_move_Z, &camera_manager, _1),
      1.0f,
  });

  key_managers.push_back(key_manager::KeyManager{
      {/*F*/ 41},
      {/*R*/ 27},
      std::bind(&HyperplaneManager::set_move_direction, &hyperplane_manager,
                _1),
      1.0f,
  });
  key_managers.push_back(key_manager::KeyManager{
      {/*O*/ 32},
      {/*L*/ 46},
      std::bind(&HyperplaneManager::set_rot_X, &hyperplane_manager, _1),
      1.0f,
  });
  key_managers.push_back(key_manager::KeyManager{
      {/*P*/ 33},
      {/*ö*/ 47},
      std::bind(&HyperplaneManager::set_rot_Y, &hyperplane_manager, _1),
      1.0f,
  });
  key_managers.push_back(key_manager::KeyManager{
      {/*ü*/ 34},
      {/*ä*/ 48},
      std::bind(&HyperplaneManager::set_rot_Z, &hyperplane_manager, _1),
      1.0f,
  });
  key_managers.push_back(key_manager::KeyManager{
      {/*¨*/ 35},
      {/*$*/ 51},
      std::bind(&HyperplaneManager::set_rot_W, &hyperplane_manager, _1),
      1.0f,
  });
}

}  // namespace viz