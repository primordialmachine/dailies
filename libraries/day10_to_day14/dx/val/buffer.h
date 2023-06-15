#if !defined(DX_BUFFER_H_INCLUDED)
#define DX_BUFFER_H_INCLUDED

#include "dx/val/context.h"

typedef struct dx_buffer dx_buffer;

static inline dx_buffer* DX_BUFFER(void* p) {
  return (dx_buffer*)p;
}

struct dx_buffer {
  dx_object _parent;
  dx_context* context;
  int (*set_data)(dx_buffer*, void const*, size_t);
};

int dx_buffer_construct(dx_buffer* buffer, dx_context* context);

void dx_buffer_destruct(dx_buffer* buffer);

int dx_buffer_set_data(dx_buffer* buffer, void const* p, size_t n);

#endif // DX_BUFFER_H_INCLUDED
