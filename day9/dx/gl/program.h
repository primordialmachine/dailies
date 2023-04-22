#if !defined(DX_GL_PROGRAM_H_INCLUDED)
#define DX_GL_PROGRAM_H_INCLUDED

#include "dx/gl/context.h"

typedef struct dx_gl_program dx_gl_program;

static inline dx_gl_program* DX_GL_PROGRAM(void* p) {
  return (dx_gl_program*)p;
}

struct dx_gl_program {
  dx_object _parent;
  GLuint vertex_program_id;
  GLuint fragment_program_id;
  GLuint program_id;
  dx_gl_context* ctx;
};

dx_gl_program* dx_gl_program_create(dx_gl_context* ctx,
                                    char const* vertex_program_p,
                                    size_t vertex_program_n,
                                    char const* fragment_program_p,
                                    size_t fragment_program_n);

#endif // DX_GL_PROGRAM_H_INCLUDED
