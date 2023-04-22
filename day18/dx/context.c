#include "dx/context.h"

int dx_context_construct(dx_context* context) {
  context->bind_texture = NULL;
  context->create_vbinding = NULL;
  context->create_buffer = NULL;
  context->create_program = NULL;
  context->create_texture = NULL;
  context->execute_commands = NULL;
  DX_OBJECT(context)->destruct = (void(*)(dx_object*)) & dx_context_destruct;
  return 0;
}

void dx_context_destruct(dx_context* context)
{/*Intentionally empty.*/}

int dx_context_bind_texture(dx_context* context, size_t unit, dx_texture* texture) {
  return context->bind_texture(context, unit, texture);
}

dx_buffer* dx_context_create_buffer(dx_context* context) {
  return context->create_buffer(context);
}

dx_vbinding* dx_context_create_vbinding(dx_context* context, DX_VERTEX_FORMAT vertex_format, dx_buffer* buffer) {
  return context->create_vbinding(context, vertex_format, buffer);
}

dx_program* dx_context_create_program(dx_context* context, dx_program_text* program_text) {
  return context->create_program(context, program_text);
}

dx_texture* dx_context_create_texture(dx_context* context) {
  return context->create_texture(context);
}

int dx_context_execute_commands(dx_context* context, dx_command_list* commands) {
  return context->execute_commands(context, commands);
}
