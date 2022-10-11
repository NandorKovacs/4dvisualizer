#version 410

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

in vec4 new_color;

void main(void) {
  color = new_color;
}