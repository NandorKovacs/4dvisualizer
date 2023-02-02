#version 430

out vec4 color;

in vec4 new_color;

void main(void) {
  color = new_color;
}