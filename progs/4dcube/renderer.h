#ifndef VIZ_RENDERER_H
#define VIZ_RENDERER_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "../../geometry/intersect.h"
#include "../../lib/camera_manager.h"
#include "../../lib/hyperplane_manager.h"
#include "../../lib/light.h"

#define NUM_VAOs 1
#define NUM_VBOs 3

namespace viz {

class Renderer {
 public:
  Renderer(glm::ivec2 window_size, CameraManager& camera_manager,
           HyperplaneManager& hyperplane_manager);

  void render();
  void set_size(glm::ivec2 size);

 private:
  void setup_vertices();
  void setup_aspect_ratio(GLuint const& program);
  void setup_light();

  void send_triangles();
  void send_lines();

  GLuint vao[NUM_VAOs];
  GLuint vbo[NUM_VBOs];
  GLuint prog, wireframe_prog;

  GLuint mvLoc, projLoc;
  glm::ivec2 window_size;
  float aspect;
  glm::mat4 pMat, mMat, mvMat;

  glm::vec3 cube_loc = glm::vec3(0.0f, 0.0f, -8.0f);

  std::vector<float> triangle_vertices;
  std::vector<float> triangle_normals;

  std::vector<float> v_lines;

  CameraManager& camera_manager;
  HyperplaneManager& hyperplane_manager;

  // ----- lighting stuff

  Material gold_material{{glm::vec4{0.2473f, 0.1995f, 0.0745f, 1},
                          glm::vec4{0.7516f, 0.6065f, 0.2265f, 1},
                          glm::vec4{0.6283f, 0.5558f, 0.3661f, 1}},
                         51.2f};

  glm::vec4 global_ambient{0.2f, 0.2f, 0.2f, 1.0f};

  DirectionalLight directional_light{{
                                         glm::vec4{0.5f, 0.5f, 0.5f, 1.0f},
                                         glm::vec4{0.5f, 0.5f, 0.5f, 1.0f},
                                         glm::vec4{0.5f, 0.5f, 0.5f, 1.0},
                                     },
                                     {0.3f, 1.0f, 0.0f}};

  glm::mat4 normal_matrix_transform;

  GLuint material_mat_loc, material_shine_loc, global_ambient_loc,
      directional_mat_loc, directional_direction_loc,
      normal_matrix_transform_loc;
  // --------------------
};

}  // namespace viz
#endif  // VIZ_RENDERER_H