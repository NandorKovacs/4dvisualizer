#version 430

out vec4 color;

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

in vec3 transformed_normal_vector;
in vec3 light_vector;
in vec3 view_vector;

void main(void) {
  vec3 N = normalize(transformed_normal_vector);
  vec3 L = normalize(light_vector);
  vec3 V = normalize(-view_vector);

  vec3 R = normalize(reflect(-L, N));

  float cosTheta = dot(L, N);
  float cosPhi = dot(V,R);

  vec3 ambient = ((global_ambient * material.mat[0]) + (directional_light.mat[0] * material.mat[0])).xyz;
  vec3 diffuse = directional_light.mat[1].xyz * material.mat[1].xyz * max(cosTheta, 0.0);
  vec3 specular = directional_light.mat[2].xyz * material.mat[2].xyz * pow(max(cosPhi, 0.0), material.shine);

  color = vec4((ambient + diffuse + specular), 1.0);
//  color = new_color;
}