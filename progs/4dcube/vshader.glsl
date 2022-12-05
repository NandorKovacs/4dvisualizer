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


// out vec4 new_color;

out vec3 transformed_normal_vector;
out vec3 light_vector;
out vec3 view_vector;

// vec3 hsv2rgb(vec3 c)
// {
//     vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//     vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//     return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
// }
// 
// vec4 getColor(float c) {
//   int ci = int(c);
// 
//   if (c == 0) {
//     return vec4(0, 0, 1, 1);
//   } else {
//     return vec4(1, 0, 0, 1);
//   }
// 
//   // float h = (ci * 17) % 256;
//   // return vec4(hsv2rgb(vec3(h / 255.0, 1, 1)), 1);
// }

void main(void){
// vec3 pos = position.xyz;
// float c = position.w;

  view_vector = (mv_matrix * vec4(pos, 1)).xyz;
  light_vector = directional_light.direction;
  transformed_normal_vector = (normal_matrix_transform * vec4(normal, 1)).xyz;

  gl_Position = proj_matrix * mv_matrix * vec4(pos, 1);
//  new_color = getColor(c);
}