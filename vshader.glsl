#version 410

layout (location=0) in vec3 position;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float time;

out vec4 new_color;

mat4 build_translate(vec3 xyz) {
  mat4 buf = mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    xyz.x, xyz.y, xyz.z, 1.0
  );
  return buf;
}

mat4 build_rot_x(float angle) {
  mat4 buf = mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0, cos(angle), -sin(angle), 0.0,
    0.0, sin(angle), cos(angle), 0.0,
    0.0, 0.0, 0.0, 1.0
  );
  return buf;
}

mat4 build_rot_y(float angle) {
  mat4 buf = mat4(
    cos(angle), 0.0, sin(angle), 0.0,
    0.0, 1.0, 0.0, 0.0,
    -sin(angle), 0.0, cos(angle), 0.0,
    0.0, 0.0, 0.0, 1.0
  );
  return buf;
}

mat4 build_rot_z(float angle) {
  mat4 buf = mat4(
    cos(angle), -sin(angle), 0.0, 0.0,
    sin(angle), cos(angle), 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
  );
  return buf;
}
void main(void){
  float i = gl_InstanceID + time;

  mat4 m_matrix = build_translate(vec3(sin(203.0*i/8000.0)*403.0, cos(301.0*i/4001.0)*401.0, sin(400.0*i/6003.0)*405.0)) * build_rot_x(1000*i) * build_rot_y(1000*i) * build_rot_z(1000*i);

  mat4 mv_matrix = v_matrix * m_matrix;
  gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
  new_color = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}