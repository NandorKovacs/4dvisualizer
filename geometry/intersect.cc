#include "intersect.h"

#include <cassert>
#include <iostream>

namespace viz {
namespace intersect {

constexpr int Face::numerical_id() {
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

constexpr void Edge::make_faces() {
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
      return i == pos || pos == first_zero ? 0 : a[pos];
    };
    Face f{t(0), t(1), t(2), t(3)};
    faces[idx] = f.numerical_id();
    ++idx;
  }
}

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
  return glm::dot(pt - plane.pos, plane.coord_system[3]);
}

int intersect_edge(Intersections::iterator it, Edge const& e,
                   Hyperplane const& plane) {
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

void Intersector::intersect(std::function<void(Triangle const&)> emit,
                            Hyperplane const& plane) {
  Intersections::iterator it = intersections.pts.begin();
  for (Edge const& e : edges) {
    int n = intersect_edge(it, e, plane);

    for (int i = 0; i < n; ++i) {
      face_content_map.insert(e, intersections.count);
      ++intersections.count;
      ++it;
    }
  }
  neighbours_map.build(face_content_map);

  for (int i = 0; i < neighbours_map.count; ++i) {
    NeighboursMap::Neighbours const& ns = neighbours_map.neighbours[i];

    for (int j = 0; j < ns.count; ++j) {
      for (int k = j + 1; k < ns.count; ++k) {
        emit(Triangle{{intersections.pts[i], intersections.pts[ns.ids[j]],
                       intersections.pts[ns.ids[k]]}});
      }
    }
  }
}

void FaceContentMap::insert(Edge const& e, int idx) {
  for (int face : e.faces) {
    face_contents[face].add(idx);
  }
}

void NeighboursMap::build(FaceContentMap const& face_content_map) {
  for (FaceContentMap::FaceContent const& face_content :
       face_content_map.face_contents) {
    int count = face_content.count;
    if (count < 2) {
      continue;
    }
    insert(face_content.ids[0], face_content.ids[1]);
    if (count == 2) {
      continue;
    }
    insert(face_content.ids[0], face_content.ids[2]);
    insert(face_content.ids[1], face_content.ids[2]);
    if (count == 3) {
      continue;
    }
    insert(face_content.ids[0], face_content.ids[3]);
    insert(face_content.ids[1], face_content.ids[3]);
    insert(face_content.ids[2], face_content.ids[3]);
  }
}

void NeighboursMap::insert(int a, int b) {
  assert(a != b);
  neighbours[a].add(b);
  if (a + 1 > count) {
    count = a + 1;
  }
  neighbours[b].add(a);
  if (b + 1 > count) {
    count = b + 1;
  }
}

}  // namespace intersect
}  // namespace viz