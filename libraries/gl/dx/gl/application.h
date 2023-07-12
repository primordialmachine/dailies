#if !defined(DX_GL_APPLICATION_H_INCLUDED)
#define DX_GL_APPLICATION_H_INCLUDED

#include "dx/application.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

DX_DECLARE_OBJECT_TYPE("dx.gl.application",
                       dx_gl_application,
                       dx_gl_application)

static dx_gl_application* DX_GL_APPLICATION(void* p) {
  return (dx_gl_application*)p;
}

struct dx_gl_application {
  dx_application _parent;
  dx_msg_queue* msg_queue;
};

/// @internal
int dx_gl_application_construct(dx_gl_application* application, dx_msg_queue* msg_queue);

#endif // DX_GL_APPLICATION_H_INCLUDED
