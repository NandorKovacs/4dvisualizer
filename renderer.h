#ifndef VIZ_RENDERER_H
#define VIZ_RENDERER_H

#include <GL/glew.h>

#define NUM_VAOs 1
#define NUM_VBOs 2

class Renderer {
 public:
  void init();
  void render();
 private:
  GLuint vao[NUM_VAOs];
  GLuint vbo[NUM_VBOs];
  GLuint prog;

  float offset = 0;
  float max_offset = 1.0;
  bool direction = true;
  float inc = 0.01;
};

#endif // VIZ_RENDERER_H