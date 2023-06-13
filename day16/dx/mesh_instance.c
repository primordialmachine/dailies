#include "dx/mesh_instance.h"

#include "dx/command.h"

static dx_cbinding* create_cbinding(dx_mesh_instance* self) {
  dx_cbinding* cbinding = dx_cbinding_create();
  if (!cbinding) {
    return NULL;
  }
  dx_cbinding_set_vec4(cbinding, "vs_mesh_ambient_rgba", &self->mesh->material->ambient_color);
  dx_cbinding_set_mat4(cbinding, "vs_mesh_world_matrix", &self->world_matrix);
  // The ambient texture sampler is always the zero-th ambient texture sampler.
  // Do this regardless of wether the mesh material has an ambient texture or not.
  dx_cbinding_set_texture_index(cbinding, "ambient_texture_sampler", 0);
  return cbinding;
}

static int add_to_backend(dx_mesh_instance* self) {
  self->commands = dx_command_list_create();
  if (!self->commands) {
    return 1;
  }
  dx_cbinding* cbinding = create_cbinding(self);
  if (!cbinding) {
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
    return 1;
  }
  dx_command* command = dx_command_create_draw(self->mesh->vbinding,
                                               self->mesh->material,
                                               cbinding,
                                               self->mesh->program,
                                               0,
                                               self->mesh->asset_mesh->number_of_vertices);
  DX_UNREFERENCE(cbinding);
  cbinding = NULL;
  if (!command) {
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
    return 1;
  }
  if (dx_command_list_append(self->commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
    return 1;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  return 0;
}

static void remove_from_backend(dx_mesh_instance* self) {
  if (self->commands) {
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
  }
}

int dx_mesh_instance_construct(dx_mesh_instance* self, DX_MAT4 world_matrix, dx_mesh* mesh) {
  if (!self || !mesh) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->commands = NULL;
  self->world_matrix = world_matrix;
  self->mesh = mesh;
  DX_REFERENCE(self->mesh);
  if (add_to_backend(self)) {
    DX_UNREFERENCE(self->mesh);
    self->mesh = NULL;
    return 1;
  }
  return 0;
}

void dx_mesh_instance_destruct(dx_mesh_instance* self) {
  remove_from_backend(self);
  if (self->mesh) {
    DX_UNREFERENCE(self->mesh);
    self->mesh = NULL;
  }
}

dx_mesh_instance* dx_mesh_instance_create(DX_MAT4 world_matrix, dx_mesh* mesh) {
  dx_mesh_instance* self = DX_MESH_INSTANCE(dx_object_alloc(sizeof(dx_mesh_instance)));
  if (!self) {
    return NULL;
  }
  if (dx_mesh_instance_construct(self, world_matrix, mesh)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

dx_cbinding* dx_mesh_instance_get_cbinding(dx_mesh_instance* self) {
  dx_command* command = dx_command_list_get_at(self->commands, 0);
  return command->draw_command.cbinding;
}

int dx_mesh_instance_update_cbinding(dx_mesh_instance* self, dx_cbinding* cbinding) {
  dx_cbinding_set_vec4(cbinding, "vs_mesh_ambient_rgba", &self->mesh->material->ambient_color);
  dx_cbinding_set_mat4(cbinding, "vs_mesh_world_matrix", &self->world_matrix);
  dx_cbinding_set_texture_index(cbinding, "ambient_texture_sampler", 0);
  return 0;
}