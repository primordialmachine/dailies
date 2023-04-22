// texture sampler.
uniform sampler2D ambient_texture_sampler;

// "fragment ambient uv" @ fragment program
in vec2 fs_fragment_ambient_uv;

// "mesh ambient color" @ fragment program
in vec4 fs_mesh_ambient_rgba;

// "fragment color" output from fragment program.
out vec4 out_fragment_color;

void main() {
  out_fragment_color = texture(ambient_texture_sampler, fs_fragment_ambient_uv);
}
