#version 410

layout (location=0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

uniform int is_wireframe;

out vec4 new_color;

void main(void){
  gl_Position = proj_matrix * mv_matrix * vec4(position, 1);
  new_color = vec4(position, 1) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
  if (is_wireframe == 1) {
    new_color = vec4(1, 1, 1, 1) - new_color;
  }
}