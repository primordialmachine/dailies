#if !defined(DX_CONTEXT_H_INCLUDED)
#define DX_CONTEXT_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.context",
                       dx_context,
                       dx_object)

static inline dx_context* DX_CONTEXT(void* p) {
  return (dx_context*)p;
}

struct dx_context {
  dx_object _parent;
  void (*clear_color)(dx_context*, float r, float g, float b, float a);
  void (*set_viewport)(dx_context*, float x, float y, float w, float h);
};

int dx_context_construct(dx_context* ctx);

void dx_context_destruct(dx_context* ctx);

void dx_context_clear_color(dx_context* ctx, float r, float g, float b, float a);

void dx_context_set_viewport(dx_context* ctx, float x, float y, float w, float h);

#endif // DX_CONTEXT_H_INCLUDED
