#version 410

layout (location=0) in vec4 position;

uniform vec4 dim_proj_origin;
uniform mat4 dim_proj_matrix;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 new_color;

void main(void){
  vec4 ld_position = dim_proj_matrix * (position - dim_proj_origin);
  ld_position.w = 1.0;

  gl_Position = proj_matrix * mv_matrix * ld_position;
  new_color =  0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}