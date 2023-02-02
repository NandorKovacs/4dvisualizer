#include "key_input.h"

#include <GL/glew.h>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <unordered_map>

using namespace std::placeholders;
namespace viz {
namespace utils {
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
  std::cout << scancode << std::endl;
}

InputHandler::InputHandler(GLFWwindow* window) {
  glfwSetKeyCallback(window, key_event_callback);
  handlers[window] = this;
  this->window = window;
}

}  // namespace utils
}  // namespace viz