/// This shader source code supports all(!) possible shaders.
/// It can be customized by define preprocessor directives.
/// The following directives are supported
///
/// - #WITH_MESH_AMBIENT_RGBA
///   if defined per-mesh ambient color coordinate is supported in this shader.
///
/// - #WITH_VERTEX_AMBIENT_RGBA
///   if defined per-vertex ambient color coordinates are supported in this shader.
///   These color values will superspeed the per-mesh ambient color coordinate (WITH_MESH_AMBIENT_RGBA).
///
/// - #WITH_VERTEX_AMBIENT_UV
///   if defined per-vertex ambient texture coordinates are supported in this shader.
///   If WITH_AMBIENT_TEXTURE is defined, then the ambient texture coordinates are used to sample color values from the ambient texture sampler.
///   These sampled colors superseed per-vertex ambient color coordinates (WITH_VERTEX_AMBIENT_RGBA).
///
/// - #WITH_AMBIENT_TEXTURE
///   if defined, an ambient texture sampler is provided.

struct dx_matrices {
  mat4 projection_matrix;
  mat4 view_matrix;
};

// mesh.model_to_world_matrix
uniform mat4 vs_mesh_world_matrix = mat4(1.);

#if defined(WITH_MESH_AMBIENT_RGBA)

// mesh.ambient.rgba
uniform vec4 vs_mesh_ambient_rgba = vec4(0.f, 255.f/255.f, 64.f/255.f, 1.f); // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette

#endif // WITH_MESH_AMBIENT_RGBA

// vertex.xyz
layout (location = 0) in vec3 vs_vertex_xyz;

#if defined(WITH_VERTEX_AMBIENT_RGBA)

// vertex.ambient.rgba
layout (location = 1) in vec4 vs_vertex_ambient_rgba;

#endif // WITH_VERTEX_AMBIENT_RGBA

#if defined(WITH_VERTEX_AMBIENT_UV)

#if defined(WITH_VERTEX_AMBIENT_RGBA)

layout (location = 2) in vec2 vs_vertex_ambient_uv;

#else

layout (location = 1) in vec2 vs_vertex_ambient_uv;

#endif // WITH_VERTEX_AMBIENT_RGBA

#endif // WITH_VERTEX_AMBIENT_UV

uniform dx_matrices matrices = dx_matrices(mat4(1.), mat4(1.));

#if defined(WITH_MESH_AMBIENT_RGBA)

// mesh ambient rgba to fragment shader
out vec4 fs_mesh_ambient_rgba;

#endif // WITH_MESH_AMBIENT_RGBA

#if defined(WITH_VERTEX_AMBIENT_RGBA)

// vertex ambient uv to fragment shader 
out vec4 fs_fragment_ambient_rgba;

#endif // WITH_VERTEX_AMBIENT_RGBA

#if defined(WITH_VERTEX_AMBIENT_UV)

// vertex ambient uv to fragment shader 
out vec2 fs_fragment_ambient_uv;

#endif // WITH_VERTEX_AMBIENT_UV

void main() {
  mat4 pvw = matrices.projection_matrix * matrices.view_matrix * vs_mesh_world_matrix;

  gl_Position = pvw * vec4(vs_vertex_xyz, 1.0);

#if defined(WITH_MESH_AMBIENT_RGBA)
  fs_mesh_ambient_rgba = vs_mesh_ambient_rgba;
#endif // WITH_MESH_AMBIENT_RGBA

#if defined(WITH_VERTEX_AMBIENT_RGBA)
  fs_vertex_ambient_rgba = vs_vertex_ambient_rgba;
#endif // WITH_VERTEX_AMBIENT_RGBA

#if defined(WITH_VERTEX_AMBIENT_UV)
  fs_fragment_ambient_uv = vs_vertex_ambient_uv;
#endif // WITH_VERTEX_AMBIENT_UV
}
