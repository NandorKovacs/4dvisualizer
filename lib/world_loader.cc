#include "world_loader.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <glm/glm.hpp>
#include <vector>

#include "../debug.h"

namespace viz {

namespace {

constexpr float pi = 3.14159265359;

glm::vec4 parse_shift(YAML::Node const& n) {
  if (!n.IsSequence() || !n.size() == 4) {
    std::ostringstream err;
    err << "Cube shift must be a vector of 4 floats. Got: " << n;
    throw std::runtime_error{err.str()};
  }
  glm::vec4 result;
  for (int i=0; i < 4; ++i) {
    YAML::Node const& component = n[i];
    if (!component.IsScalar()) {
      std::ostringstream err;
      err << "Cube shift must be a vector of 4 floats. Got: " << component;
      throw std::runtime_error{err.str()};
    }
    result[i] = component.as<float>();
  }
  return result;
}

float parse_scale(YAML::Node const& n) {
  if (!n.IsScalar()) {
    std::ostringstream err;
    err << "The scale must be a single float. Got: " << n;
    throw std::runtime_error{err.str()};
  }
  return n.as<float>();
}

std::vector<math::Rotation> parse_rotation(YAML::Node const& n) {
  if (!n.IsMap()) {
    std::ostringstream err;
    err << "The rotate key must contain a map. Got: " << n;
    throw std::runtime_error{err.str()};
  }
  std::vector<math::Rotation> res;
  for (auto rot_it = n.begin(); rot_it != n.end(); ++rot_it) {
    std::string key = rot_it->first.as<std::string>();
    float angle = rot_it->second.as<float>() / 360 * 2 * pi;
    math::Plane p = math::planeFromString(key);
    res.emplace_back(p, angle);
  }
  return res;
}

}  // namespace

World::World(std::string file_name) {
  YAML::Node config = YAML::LoadFile(file_name);
  if (!config.IsSequence()) {
    throw std::runtime_error{"The config should be a list."};
  }
  for (auto it = config.begin(); it != config.end(); ++it) {
    if (!it->IsMap()) {
      std::ostringstream err;
      err << "The config should be a list of objects, with one key (cube). Got: " << *it;
      throw std::runtime_error{err.str()};
    }
    for (auto props = it->begin(); props != it->end(); ++props) {
      std::string key = props->first.as<std::string>();
      if (key != "cube") {
        std::ostringstream err;
        err << "The top level items should be of type 'cube'. Got: " << key;
        throw std::runtime_error{err.str()};
      }
      YAML::Node cube = props->second;
      if (!cube.IsMap()) {
        std::ostringstream err;
        err << "A cube needs to be of type map. Got: " << cube;
      }

      glm::vec4 translate{0.0f};
      float scaling = 1.0f;
      std::vector<math::Rotation> rotations;

      for (auto cube_props = cube.begin(); cube_props != cube.end(); ++cube_props) {
        std::string key = cube_props->first.as<std::string>();
        if (key == "shift") {
          translate = parse_shift(cube_props->second);
        } else if (key == "scale") {
          scaling = parse_scale(cube_props->second);
        } else if (key == "rotation") {
          rotations = parse_rotation(cube_props->second);
        } else {
          std::ostringstream err;
          err << "Unknown key for cube: " << key << ", cube: " << cube;
          throw std::runtime_error{err.str()};
        }
      }

      glm::mat4 rot(1.0f);
      for (auto const& r: rotations) {
        rot = rotate(r.plane, r.angle) * rot;
      }

      cubes.emplace_back(math::Transformation{rot, scaling, translate});
    }
  }

}


}  // namespace viz
