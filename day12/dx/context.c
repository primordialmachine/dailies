#include "dx/context.h"

int dx_context_construct(dx_context* context) {
  DX_OBJECT(context)->destruct = (void(*)(dx_object*)) & dx_context_destruct;
  return 0;
}

void dx_context_destruct(dx_context* context)
{/*Intentionally empty.*/}

dx_buffer* dx_context_create_buffer(dx_context* context) {
  return context->create_buffer(context);
}

dx_vbinding* dx_context_create_vbinding(dx_context* context, dx_buffer* buffer) {
  return context->create_vbinding(context, buffer);
}

dx_program* dx_context_create_program(dx_context* context, dx_program_text* program_text) {
  return context->create_program(context, program_text);
}

int dx_context_execute_commands(dx_context* context, dx_command_list* commands) {
  return context->execute_commands(context, commands);
}
