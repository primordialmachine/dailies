#if !defined(DX_GL_TEXTURE_H_INCLUDED)
#define DX_GL_TEXTURE_H_INCLUDED

#include "dx/val/texture.h"
#include "dx/gl/context.h"


typedef struct dx_gl_texture dx_gl_texture;
static inline dx_gl_texture* DX_GL_TEXTURE(void* p) {
  return (dx_gl_texture*)p;
}

int dx_gl_texture_construct(dx_gl_texture* texture, dx_gl_context* context);

void dx_gl_texture_destruct(dx_gl_texture* texture);

dx_gl_texture* dx_gl_texture_create(dx_gl_context* context);

struct dx_gl_texture {
  dx_texture _parent;
  GLuint id;
};

#endif // DX_GL_TEXTURE_H_INCLUDED
