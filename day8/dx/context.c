#include "dx/context.h"

int dx_context_construct(dx_context* ctx) {
  ctx->clear_color = NULL;
  ctx->set_viewport = NULL;
  DX_OBJECT(ctx)->destruct = (void(*)(dx_object*)) & dx_context_destruct;
  return 0;
}

void dx_context_destruct(dx_context* ctx)
{/*Intentionally empty.*/}

void dx_context_clear_color(dx_context* ctx, float r, float g, float b, float a) {
  ctx->clear_color(ctx, r, g, b, a);
}

void dx_context_set_viewport(dx_context* ctx, float x, float y, float w, float h) {
  ctx->set_viewport(ctx, x, y, w, h);
}
