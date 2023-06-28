#include "dx/val/vbinding.h"

#include "dx/val/buffer.h"

DX_DEFINE_OBJECT_TYPE("dx.vbinding",
                      dx_vbinding,
                      dx_object)

int dx_vbinding_construct(dx_vbinding* self, dx_buffer* buffer) {
  dx_rti_type* _type = dx_vbinding_get_type();
  if (!_type) {
    return 1;
  }
  self->buffer = buffer;
  DX_REFERENCE(self->buffer);
  self->context = buffer->context;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_vbinding_destruct(dx_vbinding* self) {
  if (self->buffer) {
    DX_UNREFERENCE(self->buffer);
    self->buffer = NULL;
  }
  self->context = NULL;
}

int dx_vbinding_activate(dx_vbinding* self) {
  return self->activate(self);
}
