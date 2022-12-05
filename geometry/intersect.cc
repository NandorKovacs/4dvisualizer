#include "intersect.h"

#include <bitset>
#include <cassert>
#include <cmath>
#include <iostream>

#include "../debug.h"

namespace viz {
namespace intersect {

void VisitedTriangles::set(int i, int j, int k) {
  data[i + max_intersections * j + max_intersections * max_intersections * k] =
      true;
  data[i + max_intersections * k + max_intersections * max_intersections * j] =
      true;
  data[j + max_intersections * k + max_intersections * max_intersections * i] =
      true;
  data[j + max_intersections * i + max_intersections * max_intersections * k] =
      true;
  data[k + max_intersections * i + max_intersections * max_intersections * j] =
      true;
  data[k + max_intersections * j + max_intersections * max_intersections * i] =
      true;
}

bool VisitedTriangles::get(int i, int j, int k) {
  return data[i + max_intersections * j +
              max_intersections * max_intersections * k];
}

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
                   Hyperplane const& plane, glm::mat4x3 const& dim_transform) {
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
  *it = dim_transform * (x - plane.pos);
  ++res;
  ++it;
  return res;
}

glm::vec3 Intersector::triangle_normal(int i, int j, int k) {
  return glm::normalize(glm::cross(ipt(j) - ipt(i), ipt(k) - ipt(i)));
}

void Intersector::sweep(int i, int j, int k,
                        std::function<void(Triangle const&, glm::vec3 const& normal)> emit,
                        VisitedTriangles& visited_triangles, glm::vec3 const& center) {
  auto emit_with_normal = [&](Triangle const& t){
    glm::vec3 normal = {glm::cross(t.pts[0] - t.pts[1], t.pts[2] - t.pts[1])};
    glm::vec3 t_center = (t.pts[0] + t.pts[1] + t.pts[2]) / 3.0f;

    if (glm::dot(center - t_center, normal) > 0) {
      normal = normal * -1.0f;
    }
    emit(t, normal);
  };
  
  std::bitset<max_intersections> visited;

  if (visited_triangles.get(i, j, k)) {
    return;
  }

  visited[i] = true;
  visited[j] = true;
  visited[k] = true;

  glm::vec3 start_normal = triangle_normal(i, j, k);

  int prev = i;
  int c = j;
  while (true) {
    int best_n = -1;
    float largest_proj;
    bool could_close = false;
    for (int ii = 0; ii < neighbours_map.neighbours[c].count; ++ii) {
      int n = neighbours_map.neighbours[c].ids[ii];

      if (n == k) {
        could_close = true;
      }
      if (visited[n]) {
        continue;
      }
      float proj =
          std::fabs(glm::dot(start_normal, triangle_normal(prev, c, n)));

      if (proj <= proj_equality_criteria) {
        continue;
      }

      if (best_n == -1 || largest_proj < proj) {
        best_n = n;
        largest_proj = proj;
        continue;
      }
    }

    if (best_n == -1) {
      if (!could_close) {
        std::cerr << "sweep failed, couldnt find viable neighbours"
                  << std::endl;
      }
      break;
    }

    visited_triangles.set(prev, c, best_n);
    emit_with_normal(Triangle{ipt(i), ipt(c), ipt(best_n)});
    prev = c;
    c = best_n;
    visited[c] = true;
  }

  visited_triangles.set(prev, c, k);
  visited_triangles.set(c, k, i);
  emit_with_normal(Triangle{ipt(c), ipt(k), ipt(i)});
}

void Intersector::intersect(
    std::function<void(Triangle const&, glm::vec3 const& normal)> emit,
    Hyperplane const& plane) {
  glm::mat4x3 dim_transform = glm::transpose(plane.coord_system);

  Intersections::iterator it = intersections.pts.begin();
  glm::vec3 center{0.0f, 0.0f, 0.0f};
  for (Edge const& e : edges) {
    int n = intersect_edge(it, e, plane, dim_transform);

    for (int i = 0; i < n; ++i) {
      face_content_map.insert(e, intersections.count);
      ++intersections.count;

      center += *it;
      ++it;
    }
  }
  center = center / float(intersections.count);

  neighbours_map.build(face_content_map);

  VisitedTriangles visited_triangles;

  for (int i = 0; i < neighbours_map.count; ++i) {
    NeighboursMap::Neighbours const& ns = neighbours_map.neighbours[i];

    for (int j = 0; j < ns.count; ++j) {
      for (int k = j + 1; k < ns.count; ++k) {
        sweep(i, ns.ids[j], ns.ids[k], emit, visited_triangles, center);
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