#if defined(WITH_AMBIENT_TEXTURE)

// texture sampler.
uniform sampler2D ambient_texture_sampler;

#endif // WITH_AMBIENT_TEXTURE

#if defined(WITH_VERTEX_AMBIENT_RGBA)

// vs.vertex.ambient.rgba -> fs.fragment.ambient.rgba
in vec4 fs_fragment_ambient_rgba;

#endif // WITH_VERTEX_AMBIENT_RGBA

#if defined(WITH_VERTEX_AMBIENT_UV)

// vs.vertex.ambient.uv -> fs.fragment.ambient.uv
in vec2 fs_fragment_ambient_uv;

#endif // WITH_VERTEX_AMBIENT_UV

#if defined(WITH_MESH_AMBIENT_RGBA)

// vs.mesh.ambient.rgba -> fs.mesh.ambient.rgba
in vec4 fs_mesh_ambient_rgba;

#endif // WITH_MESH_AMBIENT_RGBA

// "fragment color" output from fragment program.
out vec4 out_fragment_color;

void main() {
#if defined(WITH_VERTEX_AMBIENT_UV) && defined(WITH_AMBIENT_TEXTURE)
  out_fragment_color = texture(ambient_texture_sampler, fs_fragment_ambient_uv);
#elif defined(WITH_VERTEX_AMBIENT_RGBA)
  out_fragment_color = fs_vertex_ambient_rgba;
#elif defined(WITH_MESH_AMBIENT_RGBA)
  out_fragment_color = fs_mesh_ambient_rgba;
#else
  #error unsupported combination of program configurations
#endif
}
