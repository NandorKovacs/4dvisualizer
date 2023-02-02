#ifndef VIZ_RENDERER_H
#define VIZ_RENDERER_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "../../geometry/intersect.h"
#include "../../lib/camera_manager.h"
#include "../../lib/hyperplane_manager.h"
#include "../../lib/light.h"
#include "../../lib/world_loader.h"

#define NUM_VAOs 1
#define NUM_VBOs 4

// Forward declaration for the text drawing library (which must be included in
// the c++ module.)
class GLTtext;

namespace viz {

class Renderer {
 public:
  Renderer(glm::ivec2 window_size, CameraManager& camera_manager,
           HyperplaneManager& hyperplane_manager, World const& world);

  ~Renderer();

  void render();
  void set_size(glm::ivec2 size);

  bool is_color = false;

 private:
  void setup_vertices();
  void setup_basic_projection(GLuint const mv_loc, GLuint const proj_loc);
  void setup_light();

  void send_triangles();
  void send_lines();
  void draw_hud();

  World const& world;

  GLuint vao[NUM_VAOs];
  GLuint vbo[NUM_VBOs];
  GLuint light_prog, wireframe_prog, color_prog;

  glm::ivec2 window_size;
  float aspect;
  glm::mat4 pMat, mMat, mvMat;

  glm::vec3 cube_loc = glm::vec3(0.0f, 0.0f, -8.0f);

  std::vector<float> triangle_vertices;
  std::vector<float> triangle_normals;
  std::vector<float> v_lines;

  CameraManager& camera_manager;
  HyperplaneManager& hyperplane_manager;

  // ----- wireframe program locations
  struct WireframeLoc {
    GLuint mv, proj;
  };
  WireframeLoc wireframe_loc;

  // ----- lighting stuff

  // glm::vec4 colors[8] = {
  //     glm::vec4{255, 255, 255, 1}, glm::vec4{137, 18, 20, 1},
  //     glm::vec4{13, 72, 172, 1},   glm::vec4{255, 85, 37, 1},
  //     glm::vec4{25, 155, 76, 1},   glm::vec4{254, 213, 47, 1},
  //     glm::vec4{255, 192, 203, 1}, glm::vec4{128, 0, 128, 1}};

  // std::vector<float> colors{255, 255, 255, 1,   137, 18, 20,  1,  13,  72,  172,
  //                           1,   255, 85,  37,  1,   25, 155, 76, 1,   254, 213,
  //                           47,  1,   255, 192, 203, 1,  128, 0,  128, 1};

  Material gold_material{{glm::vec4{0.2473f, 0.1995f, 0.0745f, 1},
                          glm::vec4{0.7516f, 0.6065f, 0.2265f, 1},
                          glm::vec4{0.6283f, 0.5558f, 0.3661f, 1}},
                         /* TODO: restore: 51.2f */ 51.2f};

  DirectionalLight directional_light{{
                                         glm::vec4{0.5f, 0.5f, 0.5f, 1.0f},
                                         glm::vec4{0.5f, 0.5f, 0.5f, 1.0f},
                                         glm::vec4{0.5f, 0.5f, 0.5f, 1.0},
                                     },
                                     {0.3f, 1.0f, 0.0f}};

  glm::vec4 global_ambient{0.2f, 0.2f, 0.2f, 1.0f};

  glm::mat4 normal_matrix_transform;

  struct LightLoc {
    GLuint mv, proj;

    GLuint material_mat, material_shine, global_ambient, directional_mat,
        directional_direction, normal_matrix_transform;
  };
  LightLoc light_loc;

  // ------ HUD
  GLTtext* text1;
};

}  // namespace viz
#endif  // VIZ_RENDERER_H