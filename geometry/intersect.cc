#include "intersect.h"

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

struct Intersections {
  int count;
  
  typedef std::array<glm::vec4, max_intersections> vec_t;
  typedef vec_t::iterator iterator;
  
  vec_t pts;
};

struct FaceContentMap {
  struct FaceContent {
    int count;
    std::array<int, 4> ids;
  };
  std::array<FaceContent, 24> face_content;
};

struct NeighboursMap {
  struct Neighbours {
    int count;
    std::array<int, max_neghbours> ids;
  };
  std::array<Neighbours, max_intersections> neighbours;
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
  inline int t(int a) { return a == 1 ? a : 0; }
  constexpr int numerical_id() {
    if (a == 0 && b == 0) {
      return 2 * t(c) + t(d);
    } else if (a == 0 && c == 0) {
      return 4 + 2 * t(b) + t(d);
    } else if (a == 0 && d == 0) {
      return 8 + 2 * t(b) + t(c);
    } else if (b == 0 && c == 0) {
      return 12 + 2 * t(a) + t(d);
    } else if (b == 0 && d == 0) {
      return 16 + 2 * t(a) + t(c);
    } else if (c == 0 && d == 0) {
      return 20 + 2 * t(a) + t(b);
    }
    return -1;
  };

 private:
  float a, b, c, d;
};

struct Edge {
  constexpr Edge() : a{}, b{}, faces{} {}
  constexpr void make_faces() {
    int first_zero = [&]() {
      for (int i = 0; i < 4; ++i) {
        if (a[i] != b[i]) {
          return i;
        }
      }
      return -1;
    }();

    int idx = 0;

    for (int i = 0; i < 4; ++i) {
      if (i == first_zero) {
        continue;
      }
      auto t = [&](int pos) {
        return i == pos || i == first_zero ? 0 : a[pos];
      };
      Face f{t(0), t(1), t(2), t(3)};
      faces[idx] = f.numerical_id();
      ++idx;
    }
  }
  glm::vec4 a, b;
  std::array<int, 3> faces;
};

constexpr std::array<Edge, 32> make_edges() {
  std::array<Edge, 32> res;

  int idx = 0;

  for (int i = 0; i < 4; ++i) {
    for (int j = -1; j < 2; j += 2) {
      for (int k = -1; k < 2; k += 2) {
        for (int l = -1; l < 2; l += 2) {
          Edge& e = res[idx];
          switch (i) {
            case 0:
              e.a = {-1, j, k, l};
              e.b = {1, j, k, l};
              break;
            case 1:
              e.a = {j, -1, k, l};
              e.b = {j, 1, k, l};
              break;
            case 2:
              e.a = {j, k, -1, l};
              e.b = {j, k, 1, l};
              break;
            case 3:
              e.a = {j, k, l, -1};
              e.b = {j, k, l, 1};
              break;
          }
          e.make_faces();
          ++idx;
        }
      }
    }
  }

  return res;
}
constexpr std::array<Edge, 32> edges = make_edges();

inline float distance(glm::vec4 const& pt, Hyperplane const& plane) {
  return glm::dot(pt - plane.pos, plane.normal);
}

int intersect_edge(Intersections::iterator it, Edge const& e, Hyperplane const& plane) {
  float da = distance(e.a, plane);
  float db = distance(e.b, plane);

  int res = 0;

  if (da * db > 0) {
    return res;
  }

  if (da == 0) {
    *it = e.a;
    ++res;
    ++it;
  }
  if (db == 0) {
    *it = e.b;
    ++res;
    ++it;
  }
  if (res > 0) {
    return res;
  }
  da = std::fabs(da);
  db = std::fabs(db);

  glm::vec4 x = e.a + (e.b - e.a) * (da / (da + db));
  *it = x;
  ++res;
  ++it;
  return res;
}

int intersect(std::array<PointOnEdge, 32>& res, Hyperplane const& plane) {
  
}

}  // namespace intersect
}  // namespace viz