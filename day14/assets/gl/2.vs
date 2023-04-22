struct dx_matrices {
  mat4 projection_matrix;
  mat4 view_matrix;
  mat4 world_matrix;
};

struct dx_mesh {
  vec4 color;
};

#define WITH_VERTEX_COLOR

layout (location = 0) in vec3 position;
#if defined(WITH_VERTEX_COLOR)
layout (location = 1) in vec4 color;
#endif

uniform dx_mesh mesh = dx_mesh(vec4(0.f, 255.f/255.f, 64.f/255.f, 1.0f)); // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
uniform dx_matrices matrices = dx_matrices(mat4(1.), mat4(1.), mat4(1.));

// "fragment color" input to fragment shader.
out vec4 fragmentColor2; 

void main() {
  mat4 pvw = matrices.projection_matrix * matrices.view_matrix * matrices.world_matrix;
  gl_Position = pvw * vec4(position.x, position.y, position.z, 1.0);
#if defined(WITH_VERTEX_COLOR)
  fragmentColor2 = color;
#else
  fragmentColor2 = mesh.color;
#endif
}
