#include "dx/gl/context.h"

static void clear_color(dx_gl_context* ctx, float r, float g, float b, float a) {
  ctx->glClearColor(r, g, b, a);
  ctx->glClear(GL_COLOR_BUFFER_BIT);
}

static void set_viewport(dx_gl_context* ctx, float x, float y, float w, float h) {
  ctx->glViewport(x, y, w, h);
}

int dx_gl_context_construct(dx_gl_context* ctx, void *(*link)(char const *name)) {
  if (dx_context_construct(DX_CONTEXT(ctx))) {
    return 1;
  }
#define DEFINE(TYPE, NAME) (ctx)->NAME = NULL;
#include "dx/gl/functions.i"
#undef DEFINE

#define DEFINE(TYPE, NAME) \
  (ctx)->NAME = (TYPE)link(#NAME); \
  if (!(ctx)->NAME) { \
    dx_log("unable to link " #NAME "\n", sizeof("unable to link " #NAME "\n")); \
    return 1; \
  }
#include "dx/gl/functions.i"
#undef DEFINE
  DX_CONTEXT(ctx)->clear_color = (void (*)(dx_context*,float, float, float, float)) & clear_color;
  DX_CONTEXT(ctx)->set_viewport = (void (*)(dx_context*,float, float, float, float)) & set_viewport;
  DX_OBJECT(ctx)->destruct = (void(*)(dx_object*)) & dx_gl_context_destruct;
  return 0;
}

void dx_gl_context_destruct(dx_gl_context* ctx) {
#define DEFINE(TYPE, NAME) (ctx)->NAME = NULL;
#include "dx/gl/functions.i"
#undef DEFINE
  dx_context_destruct(DX_CONTEXT(ctx));
}
