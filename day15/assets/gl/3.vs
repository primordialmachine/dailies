struct dx_matrices {
  mat4 projection_matrix;
  mat4 view_matrix;
};

// mesh.model_to_world_matrix
uniform mat4 vs_mesh_world_matrix = mat4(1.);
// mesh.ambient.rgba
uniform vec4 vs_mesh_ambient_rgba = vec4(0.f, 255.f/255.f, 64.f/255.f, 1.f); // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
layout (location = 0) in vec3 vs_vertex_xyz;
layout (location = 1) in vec2 vs_vertex_ambient_uv;

uniform dx_matrices matrices = dx_matrices(mat4(1.), mat4(1.));

// mesh ambient rgba to fragment shader
out vec4 fs_mesh_ambient_rgba;

// vertex ambient uv to fragment shader 
out vec2 fs_fragment_ambient_uv;

void main() {
  mat4 pvw = matrices.projection_matrix * matrices.view_matrix * vs_mesh_world_matrix;
  gl_Position = pvw * vec4(vs_vertex_xyz, 1.0);
  fs_fragment_ambient_uv = vs_vertex_ambient_uv;
  fs_mesh_ambient_rgba = vs_mesh_ambient_rgba;
}
