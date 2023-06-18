#if !defined(DX_GL_BUFFER_H_INCLUDED)
#define DX_GL_BUFFER_H_INCLUDED

#include "dx/val/buffer.h"
#include "dx/gl/context.h"

typedef struct dx_gl_buffer dx_gl_buffer;

static inline dx_gl_buffer* DX_GL_BUFFER(void* p) {
  return (dx_gl_buffer*)p;
}

struct dx_gl_buffer {
  dx_buffer _parent;
  GLuint id;
};

dx_gl_buffer* dx_gl_buffer_create(dx_gl_context* ctx);

#endif // DX_GL_BUFFER_H_INCLUDED
