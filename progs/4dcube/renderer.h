#ifndef VIZ_RENDERER_H
#define VIZ_RENDERER_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "../../geometry/intersect.h"
#include "../../lib/camera_manager.h"
#include "../../lib/hyperplane_manager.h"

#define NUM_VAOs 1
#define NUM_VBOs 2

namespace viz {

class Renderer {
 public:
  Renderer(glm::ivec2 window_size, CameraManager& camera_manager,
           HyperplaneManager& hyperplane_manager);

  void render();
  void set_size(glm::ivec2 size);

 private:
  void setup_vertices();
  void send_vertices();

  GLuint vao[NUM_VAOs];
  GLuint vbo[NUM_VBOs];
  GLuint prog;

  GLuint mvLoc, projLoc, dim_proj_loc, dim_proj_origin_loc;
  glm::ivec2 window_size;
  float aspect;
  glm::mat4 pMat, mMat, mvMat, dim_proj_mat;

  glm::vec3 cube_loc = glm::vec3(0.0f, 0.0f, -8.0f);

  std::vector<float> vertices;

  CameraManager& camera_manager;
  HyperplaneManager& hyperplane_manager;
};

}  // namespace viz
#endif  // VIZ_RENDERER_H