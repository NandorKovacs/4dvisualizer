#version 410

layout (location=0) in vec3 pos;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 new_color;

void main(void){
  gl_Position = proj_matrix * mv_matrix * vec4(pos, 1);

  new_color = vec4(1); 
}