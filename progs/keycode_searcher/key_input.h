#ifndef VIZ__KEY_INPUT
#define VIZ_KEY_INPUT

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace viz {
namespace utils {
class InputHandler {
 public:
  InputHandler(GLFWwindow* window);

  void on_key_action(GLFWwindow* window, int key, int scancode, int action,
                     int mods);

 private:
  GLFWwindow* window;
};
}  // namespace utils
}  // namespace viz
#endif  // VIZ_INPUT_HANDLER