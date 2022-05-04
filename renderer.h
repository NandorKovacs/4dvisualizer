#ifndef VIZ_RENDERER_H
#define VIZ_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#define NUM_VAOs 1
#define NUM_VBOs 2

class Renderer {
 public:
  void init();
  void render(GLFWwindow* window, double currentTime);

 private:
  void setup_vertices();

  GLuint vao[NUM_VAOs];
  GLuint vbo[NUM_VBOs];
  GLuint prog;

  GLuint mvLoc, projLoc;
  int width, height;
  float aspect;
  glm::mat4 pMat, vMat, mMat, mvMat, rotMat, tiltMat;

  glm::vec3 camera_loc = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 cube_loc = glm::vec3(0.0f, 0.0f, -8.0f);

  int roation_count = 0;
};

#endif  // VIZ_RENDERER_H