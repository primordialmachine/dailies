#include "dx/vbinding.h"

#include "dx/buffer.h"

int dx_vbinding_construct(dx_vbinding* vbinding, dx_buffer* buffer) {
  vbinding->buffer = buffer;
  DX_REFERENCE(vbinding->buffer);
  vbinding->context = buffer->context;
  DX_OBJECT(vbinding)->destruct = (void(*)(dx_object*)) & dx_vbinding_destruct;
  return 0;
}

void dx_vbinding_destruct(dx_vbinding* vbinding) {
  if (vbinding->buffer) {
    DX_UNREFERENCE(vbinding->buffer);
    vbinding->buffer = NULL;
  }
  vbinding->context = NULL;
}

int dx_vbinding_activate(dx_vbinding* vbinding) {
  return vbinding->activate(vbinding);
}
