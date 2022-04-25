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
};

#endif // VIZ_RENDERER_H