#include "state_debug.h"

#include <fstream>

namespace viz {
void StateDebug::dump() {
  std::ofstream ofs{"state_debug.txt"};
  write(ofs);
}

void StateDebug::load() {}

void StateDebug::write(std::ostream& os) {}
}  // namespace viz
