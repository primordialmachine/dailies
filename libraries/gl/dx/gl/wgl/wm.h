#if !defined(DX_GL_WGL_WM_H_INCLUDED)
#define DX_GL_WGL_WM_H_INCLUDED

#include "dx/core.h"
#include "dx/application.h"
#include "dx/gl/wgl/application.h"
#include "dx/gl/wgl/context.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_gl_wgl_open_wm();

int dx_gl_wgl_close_wm();

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
