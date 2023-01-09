#ifndef VIZ_WORLD_H
#define VIZ_WORLD_H

#include <string>
#include <vector>

#include "../geometry/4d_math.h"

namespace viz {

struct Cube {
  math::Transformation transformation;
};

struct World {
  World(std::string file_name);

  std::vector<Cube> cubes;
};


}  // namespace viz

#endif  // VIZ_WORLD_H
