#include "dx/val/program.h"

DX_DEFINE_OBJECT_TYPE("dx.program",
                      dx_program,
                      dx_object)

int dx_program_construct(dx_program* self, dx_context* ctx) {
  dx_rti_type* _type = dx_program_get_type();
  if (!_type) {
    return 1;
  }
  self->ctx = ctx;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_program_destruct(dx_program* self) {
  self->ctx = NULL;
}

int dx_program_activate(dx_program* self) {
  return self->activate(self);
}

int dx_program_bind(dx_program* self, dx_cbinding* cbinding) {
  return self->bind(self, cbinding);
}
