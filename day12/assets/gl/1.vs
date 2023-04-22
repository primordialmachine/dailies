struct dx_matrices {
  mat4 projection_matrix;
  mat4 view_matrix;
  mat4 world_matrix;
};

layout (location = 0) in vec3 position;

uniform dx_matrices matrices = dx_matrices(mat4(1.), mat4(1.), mat4(1.));

void main() {
  mat4 pvw = matrices.projection_matrix * matrices.view_matrix * matrices.world_matrix;
  gl_Position = pvw * vec4(position.x, position.y, position.z, 1.0);
}
