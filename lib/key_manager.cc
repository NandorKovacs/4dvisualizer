#include "key_manager.h"

#include <GLFW/glfw3.h>

namespace viz {
namespace key_manager {

void KeyManager::on_action(int key, int action) {
  auto handle =
      [&](Key& k, Key& l) {
        for (int code : k.kb.key_codes) {
          if (key == code) {
            if (action == GLFW_PRESS && !k.is_pressed) {
              k.is_pressed = true;
              k.kb.active();
            }
            if (action == GLFW_RELEASE && k.is_pressed) {
              k.is_pressed = false;

              if (l.is_pressed) {
                l.kb.active();
              } else {
                idle();
              }
            }
          }
        }
      };
  handle(a, b);
  handle(b, a);
}

}  // namespace key_manager
}  // namespace viz