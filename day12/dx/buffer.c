#include "dx/buffer.h"

int dx_buffer_construct(dx_buffer* buffer, dx_context* context) {
  buffer->context = context;
  DX_OBJECT(buffer)->destruct = (void(*)(dx_object*)) & dx_buffer_destruct;
  return 0;
}

void dx_buffer_destruct(dx_buffer* buffer) {
  buffer->context = NULL;
}

int dx_buffer_set_data(dx_buffer* buffer, void const* p, size_t n) {
  return buffer->set_data(buffer, p, n);
}
