#include "dx/program.h"

int dx_program_construct(dx_program* program, dx_context* ctx) {
  program->ctx = ctx;
  DX_OBJECT(program)->destruct = (void(*)(dx_object*)) & dx_program_destruct;
  return 0;
}

void dx_program_destruct(dx_program* program) {
  program->ctx = NULL;
}

int dx_program_activate(dx_program* program) {
  return program->activate(program);
}

int dx_program_bind(dx_program* program, dx_cbinding* cbinding) {
  return program->bind(program, cbinding);
}
