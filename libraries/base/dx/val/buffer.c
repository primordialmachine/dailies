#include "dx/val/buffer.h"

DX_DEFINE_OBJECT_TYPE("dx.buffer",
                      dx_buffer,
                      dx_object)

int dx_buffer_construct(dx_buffer* self, dx_context* context) {
  dx_rti_type* _type = dx_buffer_get_type();
  if (!_type) {
    return 1;
  }
  self->context = context;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_buffer_destruct(dx_buffer* self) {
  self->context = NULL;
}

int dx_buffer_set_data(dx_buffer* self, void const* p, size_t n) {
  return self->set_data(self, p, n);
}
