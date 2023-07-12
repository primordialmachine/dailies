#if !defined(DX_GL_WGL_APPLICATION_H_INCLUDED)
#define DX_GL_WGL_APPLICATION_H_INCLUDED

#include "dx/gl/application.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

DX_DECLARE_OBJECT_TYPE("dx.gl.wgl.application",
                       dx_gl_wgl_application,
                       dx_gl_application)

static dx_gl_wgl_application* DX_GL_WGL_APPLICATION(void* p) {
  return (dx_gl_wgl_application*)p;
}

struct dx_gl_wgl_application {
  dx_gl_application _parent;
  char* class_name;
  ATOM class_handle;
  HINSTANCE instance_handle;
};

/// @internal
int dx_gl_wgl_application_construct(dx_gl_wgl_application* application, dx_msg_queue* msg_queue);

/// @internal
/// @brief Create an instance of this application object.
/// @return A pointer to the application object on success. The null pointer on failure.
/// @default-failure
dx_gl_wgl_application* dx_gl_wgl_application_create(dx_msg_queue* msg_queue);

// returns null if the application does not exist
// otherwise increments the reference count of the application by 1
dx_gl_wgl_application* dx_gl_wgl_application_get();

#endif // DX_GL_WGL_APPLICATION_H_INCLUDED
