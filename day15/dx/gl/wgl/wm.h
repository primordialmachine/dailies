#if !defined(DX_GL_WGL_WM_H_INCLUDED)
#define DX_GL_WGL_WM_H_INCLUDED

#include "dx/core.h"
#include "dx/application.h"
#include "dx/gl/wgl/context.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct dx_gl_wgl_application dx_gl_wgl_application;

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

// Get the size of the canvas
// @param w, h
// pointers to <code>int</code> variables
// @return
// @a on success. A non-zero valueon failure
// @post
// on success:
// <code>*w</code> was assigned the width of the canvas
// <code>*h</code> were assigned the height of the canvas
// on failure:
// neither @a w nor @a h was dereferenced
int dx_gl_wgl_get_canvas_size(int *w, int *h);

// @brief Get the GL context.
// The functions remain valid until dx_gl_wgl_close_wm is called.
// @return pointer to the functions on success. null pointer on failure.
dx_gl_wgl_context* dx_gl_wgl_get_context();

// @brief Must be invoked before entering a frame.
void dx_gl_wgl_enter_frame();

/// @brief Must be invoked after leaving a frame.
void dx_gl_wgl_leave_frame();

#endif // DX_GL_WGL_WM_H_INCLUDED
