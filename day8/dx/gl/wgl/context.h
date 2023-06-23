#if !defined(DX_GL_WGL_CONTEXT_H_INCLUDED)
#define DX_GL_WGL_CONTEXT_H_INCLUDED

#include "dx/gl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.gl.wgl.context",
                       dx_gl_wgl_context,
                       dx_gl_context)

static inline dx_gl_wgl_context* DX_GL_WGL_CONTEXT(void* p) {
  return (dx_gl_wgl_context*)p;
}

struct dx_gl_wgl_context {
  dx_gl_context _parent;
};

int dx_gl_wgl_context_construct(dx_gl_wgl_context* ctx);

void dx_gl_wgl_context_destruct(dx_gl_wgl_context* ctx);

dx_gl_wgl_context* dx_gl_wgl_context_create();

#endif // DX_GL_WGL_CONTEXT_H_INCLUDED
