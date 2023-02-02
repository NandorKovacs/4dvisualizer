#version 430

layout (location=0) in vec4 pos;
layout (location=1) in vec3 normal;

uniform mat4 mvc_matrix;
uniform mat4 projc_matrix;

out vec4 new_color;

vec4 convert(vec4 c) {
  return vec4(c.x/255, c.y/255, c.z/255, c.w);
}

void main(void){
  gl_Position = projc_matrix * mvc_matrix * vec4(pos.xyz, 1);
  
  int color = int(pos.w);

  if (color == 0) {
    new_color = convert(vec4(255, 255, 255, 1));
  } else if (color == 1) {
    new_color = convert(vec4(137, 18, 20, 1));
  } else if (color == 2) {
    new_color = convert(vec4(13, 72, 172, 1));
  } else if (color == 3) {
    new_color = convert(vec4(255, 85, 37, 1));
  } else if (color == 4) {
    new_color = convert(vec4(25, 155, 76, 1));
  } else if (color == 5) {
    new_color = convert(vec4(254, 213, 47, 1));
  } else if (color == 6) {
    new_color = convert(vec4(255, 192, 203, 1));
  } else if (color == 7) {
    new_color = convert(vec4(128, 0, 128, 1));
  } else {
    new_color = convert(vec4(0));
  }
}