#include "dx/val/context.h"

DX_DEFINE_OBJECT_TYPE("dx.context",
                      dx_context,
                      dx_object)

int dx_context_construct(dx_context* self) {
  dx_rti_type* _type = dx_context_get_type();
  if (!_type) {
    return 1;
  }
  self->bind_texture = NULL;
  self->create_vbinding = NULL;
  self->create_buffer = NULL;
  self->create_program = NULL;
  self->create_texture = NULL;
  self->execute_commands = NULL;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_context_destruct(dx_context* self)
{/*Intentionally empty.*/}

int dx_context_bind_texture(dx_context* self, dx_size unit, dx_texture* texture) {
  return self->bind_texture(self, unit, texture);
}

dx_buffer* dx_context_create_buffer(dx_context* self) {
  return self->create_buffer(self);
}

dx_vbinding* dx_context_create_vbinding(dx_context* self, DX_VERTEX_FORMAT vertex_format, dx_buffer* buffer) {
  return self->create_vbinding(self, vertex_format, buffer);
}

dx_program* dx_context_create_program(dx_context* self, dx_program_text* program_text) {
  return self->create_program(self, program_text);
}

dx_texture* dx_context_create_texture(dx_context* self) {
  return self->create_texture(self);
}

int dx_context_execute_commands(dx_context* self, dx_command_list* commands) {
  return self->execute_commands(self, commands);
}
