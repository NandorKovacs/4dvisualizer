#ifndef VIZ_RENDERER_H
#define VIZ_RENDERER_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include "../../lib/camera_manager.h"

#define NUM_VAOs 1
#define NUM_VBOs 2

class Renderer {
 public:
  Renderer(glm::ivec2 window_size, CameraManager& camera_manager);

  void render();
  void set_size(glm::ivec2 size);

 private:
  void setup_vertices();

  GLuint vao[NUM_VAOs];
  GLuint vbo[NUM_VBOs];
  GLuint prog;

  GLuint mvLoc, projLoc;
  glm::ivec2 window_size;
  float aspect;
  glm::mat4 pMat, vMat, mMat, mvMat;

  glm::vec3 cube_loc = glm::vec3(0.0f, 0.0f, -8.0f);

  CameraManager& camera_manager;
};

#endif  // VIZ_RENDERER_H