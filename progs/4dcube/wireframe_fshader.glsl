#version 410

in vec4 new_color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 color;

void main(void) {
  color = new_color;
}