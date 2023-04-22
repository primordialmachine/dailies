#if !defined(DX_GL_BINDING_H_INCLUDED)
#define DX_GL_BINDING_H_INCLUDED

#include "dx/gl/context.h"
#include "dx/vbinding.h"
typedef struct dx_gl_buffer dx_gl_buffer;

typedef struct dx_gl_binding dx_gl_binding;

static inline dx_gl_binding* DX_GL_BINDING(void* p) {
  return (dx_gl_binding*)p;
}

struct dx_gl_binding {
  dx_vbinding _parent;
  DX_VERTEX_FORMAT vertex_format;
  GLuint id;
};

dx_gl_binding* dx_gl_binding_create(DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer);

#endif // DX_GL_BINDING_H_INCLUDED
