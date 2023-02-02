#ifndef VIZ_INTERSECT_H
#define VIZ_INTERSECT_H

#include <array>
#include <bitset>
#include <cstdint>
#include <functional>
#include <glm/glm.hpp>
#include <iostream>

#include "../debug.h"
#include "geometry.h"

namespace viz {
namespace intersect {

// A trivial but too high estimate of the maximum number of intersections
// between edges of the 4dcube and a hyperplane. method: max 4 per 2d face, 24
// 2d faces. The real maximum is likely much lower and memory can be optimized
// by setting it more strictly.
constexpr int max_intersections = 96;

// Also a high estimate. One point can be on maximally 6 faces, all of which can
// have a maximal number of 4 points of intersection on them -> 6*4
constexpr int max_neghbours = 24;
constexpr float proj_equality_criteria = 1 - 1e-6;

struct Intersections {
  int count = 0;

  typedef std::array<glm::vec3, max_intersections> vec_t;
  typedef vec_t::iterator iterator;

  vec_t pts;
  std::array<int, max_intersections> edge_id;

  inline void dump() {
    std::cerr << "-----intersections------" << std::endl;
    for (int i = 0; i < count; ++i) {
      std::cerr << i << ": " << pts[i] << std::endl;
    }
    std::cerr << "------------------------" << std::endl;
  }
};

struct Edge {
  constexpr Edge() : a{}, b{}, faces{} {}
  constexpr void make_faces();
  glm::vec4 a, b;
  std::array<int, 3> faces;
};

template <int N>
void append_unique(std::array<int, N>& a, int& count, int x) {
  for (int i = 0; i < count; ++i) {
    if (a[i] == x) {
      return;
    }
  }

  a[count] = x;
  ++count;
}

struct FaceContentMap {
  struct FaceContent {
    int count = 0;
    inline void add(int x) { append_unique<4>(ids, count, x); }
    std::array<int, 4> ids;
  };
  void insert(Edge const& e, int idx);

  std::array<FaceContent, 24> face_contents;
};

struct NeighboursMap {
  int count = 0;
  struct Neighbours {
    int count = 0;
    inline void add(int x) { append_unique<max_neghbours>(ids, count, x); }
    std::array<int, max_neghbours> ids;
  };
  void build(FaceContentMap const& face_content_map);
  void insert(int a, int b);

  std::array<Neighbours, max_intersections> neighbours;

  inline void dump() {
    std::cerr << "-----neighbours_map------" << std::endl;
    for (int i = 0; i < count; ++i) {
      std::cerr << i << ": ";
      for (int j = 0; j < neighbours[i].count; ++j) {
        std::cerr << "j: " << neighbours[i].ids[j] << " ";
      }
      std::cerr << std::endl;
    }
    std::cerr << "------------------------" << std::endl;
  }
};

class Face {
 public:
  /**
   * @brief Construct a new Face object
   *
   * @param abcd +1, -1, or 0. Zero indicates unspecified coordinates. The 4
   * corners of the face can be computed by substituting the zeros with all
   * combinations of +1 and -1.
   */
  constexpr Face(float a, float b, float c, float d) : a{a}, b{b}, c{c}, d{d} {}

  constexpr int numerical_id();

 private:
  inline constexpr int t(int a) { return a == 1 ? a : 0; }
  float a, b, c, d;
};

struct PointOnEdge {
  glm::vec4 coords;
  std::uint32_t faces = 0;
};

struct Triangle {
  std::array<glm::vec3, 3> pts;
};

class VisitedTriangles {
 public:
  bool get(int i, int j, int k);
  void set(int i, int j, int k);

 private:
  std::bitset<max_intersections * max_intersections * max_intersections> data;
};

class Intersector {
 public:
  /**
   * @brief Intersects the -1 +1 4d cube with an arbitrary hyperplane.
   *
   * @param res Points of intersection.
   * @param plane
   * @return int The number of valid items in res.
   */
  void intersect(
      std::function<void(Triangle const&, glm::vec3 const& normal, int color)> emit,
      Hyperplane const& plane);

 private:
  inline glm::vec3& ipt(int i) { return intersections.pts[i]; };

  glm::vec3 triangle_normal(int i, int j, int k);

  int get_color(int a, int b, int c);

  void sweep(int i, int j, int k,
             std::function<void(Triangle const&, glm::vec3 const& normal, int color)>,
             VisitedTriangles& visited_triangles, glm::vec3 const& center, int color);
  Intersections intersections;
  FaceContentMap face_content_map;
  NeighboursMap neighbours_map;
};

}  // namespace intersect
}  // namespace viz

#endif  // VIZ_INTERSECT_H