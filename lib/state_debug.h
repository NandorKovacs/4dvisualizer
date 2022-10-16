#ifndef VIZ_STATE_DEBUG_H
#define VIZ_STATE_DEBUG_H

#include "camera_manager.h"
#include "hyperplane_manager.h"

namespace viz {

class StateDebug {
 public:
  StateDebug(CameraManager& camera_manager,
             HyperplaneManager& hyperplane_manager)
      : camera_manager{camera_manager},
        hyperplane_manager{hyperplane_manager} {};

  void dump();
  void load();

  void write(std::ostream& os);
 private:
  CameraManager& camera_manager;
  HyperplaneManager& hyperplane_manager
};

}  // namespace viz

#endif  // VIZ_STATE_DEBUG_H