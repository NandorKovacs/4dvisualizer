#version 410

layout (location=0) in vec4 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

uniform int is_wireframe;

out vec4 new_color;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 getColor(float c) {
  int ci = int(c);

  if (c == 0) {
    return vec4(0, 0, 1, 1);
  } else {
    return vec4(1, 0, 0, 1);
  }

  // float h = (ci * 17) % 256;
  // return vec4(hsv2rgb(vec3(h / 255.0, 1, 1)), 1);
}

void main(void){
  vec3 pos = position.xyz;
  float c = position.w;

  gl_Position = proj_matrix * mv_matrix * vec4(pos, 1);
  
  new_color = getColor(c);
  if (is_wireframe == 1) {
    new_color = vec4(1);
  }
}