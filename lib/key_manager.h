#ifndef VIZ_KEY_MANAGER_H
#define VIZ_KEY_MANAGER_H

#include <functional>
#include <vector>

namespace viz {
namespace key_manager {

struct KeyBehavior {
  std::vector<int> key_codes;
  std::function<void()> active;
};

class KeyManager {
 public:
  KeyManager(KeyBehavior a, KeyBehavior b, std::function<void()> idle)
      : a{a, false}, b{b, false}, idle{idle} {}
  KeyManager(std::vector<int> key_codes_a, std::vector<int> key_codes_b,
             std::function<void(float)> setter, float v)
      : KeyManager{
            {
                key_codes_a,
                std::bind(setter, v),
            },
            {
                key_codes_b,
                std::bind(setter, -v),
            },
            std::bind(setter, 0),
        } {}

  void on_action(int key, int action);

 private:
  struct Key {
    KeyBehavior kb;
    bool is_pressed;
  };
  Key a, b;

  std::function<void()> idle;
};

}  // namespace key_manager
}  // namespace viz

#endif  // VIZ_KEY_MANAGER_H