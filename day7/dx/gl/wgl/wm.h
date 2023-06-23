#if !defined(DX_GL_WGL_WM_H_INCLUDED)
#define DX_GL_WGL_WM_H_INCLUDED

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "dx/core.h"
#include "dx/application.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.gl.wgl.application",
                       dx_gl_wgl_application,
                       dx_application)

static dx_gl_wgl_application* DX_GL_WGL_APPLICATION(void* p) {
  return (dx_gl_wgl_application*)p;
}

struct dx_gl_wgl_application {
  dx_application _parent;
  char* class_name;
  ATOM class_handle;
  HINSTANCE instance_handle;
  dx_msg_queue* msg_queue;
};

/// @internal
void dx_gl_wgl_application_destruct(dx_gl_wgl_application* application);

/// @internal
int dx_gl_wgl_application_construct(dx_gl_wgl_application* application, dx_msg_queue* msg_queue);

/// @internal
dx_gl_wgl_application* dx_gl_wgl_application_create(dx_msg_queue* msg_queue);

// startup the application
// fails if the application is already started up
int dx_gl_wgl_application_startup(dx_msg_queue* msg_queue);

// shutdown the application
// undefined if the application is already shutted down
void dx_gl_wgl_application_shutdown();

// returns null if the application does not exist
// otherwise increments the reference count of the application by 1
dx_gl_wgl_application* dx_gl_wgl_application_get();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_gl_wgl_open_wm();

int dx_gl_wgl_close_wm();

int dx_gl_wgl_update_wm();

#endif // DX_GL_WGL_WM_H_INCLUDED
