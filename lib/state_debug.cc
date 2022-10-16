#include "state_debug.h"

#include <fstream>

namespace viz {
void StateDebug::dump() {
  std::ofstream ofs{"state_debug.txt"};
  write(ofs);
}

void StateDebug::load() {}

void StateDebug::
}  // namespace viz

/*
C 

*/