#version 430

layout (location=0) in vec3 pos;
layout (location=1) in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

struct Material {
  mat3x4 mat;
  float shine;
};

struct DirectionalLight {
  mat3x4 mat;
  vec3 direction;
};

uniform Material material;
uniform DirectionalLight directional_light;

uniform vec4 global_ambient;

uniform mat4 normal_matrix_transform;


out vec3 transformed_normal_vector;
out vec3 light_vector;
out vec3 view_vector;


void main(void){
  view_vector = (mv_matrix * vec4(pos, 1.0)).xyz;
  light_vector = directional_light.direction - view_vector;
  transformed_normal_vector = (normal_matrix_transform * vec4(normal, 1.0)).xyz;

  gl_Position = proj_matrix * mv_matrix * vec4(pos, 1);
}