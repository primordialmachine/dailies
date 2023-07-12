#include "dx/gl/wgl/wm.h"

#include "dx/core.h"
#include "dx/gl/wgl/context.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "opengl32.lib")

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on Nvidia Optimus systems
// with up-to-date drivers
__declspec(dllexport) DWORD NvOptimusEnablement = 1;

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on AMD PowerXpress systems
// with up-to-date drivers
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct dx_gl_wgl_window dx_gl_wgl_window;

struct dx_gl_wgl_window {
  HWND wnd;
  HDC dc;
  HGLRC glrc;
  dx_gl_wgl_application* application;
};

static int dx_gl_wgl_init_wgl_v1(dx_gl_wgl_window* window, dx_gl_wgl_window*);

static int dx_gl_wgl_init_wgl_v2(dx_gl_wgl_window* window, dx_gl_wgl_window*);

static int dx_gl_wgl_open_window_internal(dx_gl_wgl_window** window, dx_gl_wgl_window* existing,
                                          int (*init_wgl)(dx_gl_wgl_window*, dx_gl_wgl_window*));

static void dx_gl_wgl_close_window_internal(dx_gl_wgl_window* window);

int dx_gl_wgl_open_window();

int dx_gl_wgl_close_window();


static dx_gl_wgl_window* g_window = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void dx_gl_wgl_close_window_internal(dx_gl_wgl_window* window) {
  if (window->glrc) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(window->glrc);
    window->glrc = NULL;
  }
  if (window->dc) {
    ReleaseDC(window->wnd, window->dc);
    window->dc = NULL;
  }
  if (window->wnd) {
    DestroyWindow(window->wnd);
    window->wnd = NULL;
  }
  if (window->application) {
    DX_UNREFERENCE(window->application);
    window->application = NULL;
  }
  dx_memory_deallocate(window);
}

static int dx_gl_wgl_open_window_internal(dx_gl_wgl_window** window, dx_gl_wgl_window* existing, int (*init_wgl)(dx_gl_wgl_window*, dx_gl_wgl_window*)) {
  dx_gl_wgl_application* application = dx_gl_wgl_application_get();
  if (!application) {
    return 1;
  }
  dx_gl_wgl_window* window1 = dx_memory_allocate(sizeof(dx_gl_wgl_window));
  if (!window1) {
    DX_UNREFERENCE(application);
    application = NULL;
    dx_log("allocation failed\n", sizeof("allocation failed\n"));
    return 1;
  }
  window1->wnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                                application->class_name,
                                "Media Evaluation Framework",
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                NULL,
                                NULL,
                                application->instance_handle,
                                NULL);
  if (!window1->wnd) {
    dx_memory_deallocate(window1);
    window1 = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create window\n", sizeof("unable to create window\n"));
    return 1;
  }
  window1->dc = GetDC(window1->wnd);
  if (!window1->dc) {
    DestroyWindow(window1->wnd);
    window1->wnd = NULL;
    dx_memory_deallocate(window1);
    window1 = NULL;
    DX_UNREFERENCE(application);
    application = NULL;
    dx_log("unable to create drawing context\n", sizeof("unable to create drawing context\n"));
    return 1;
  }

  if (init_wgl) {
    if (init_wgl(window1, existing)) {
      ReleaseDC(window1->wnd, window1->dc);
      window1->dc = NULL;
      DestroyWindow(window1->wnd);
      window1->wnd = NULL;
      dx_memory_deallocate(window1);
      window1 = NULL;
      DX_UNREFERENCE(application);
      application = NULL;
      return 1;
    }
  }

  window1->application = application;

  ShowWindow(window1->wnd, SW_SHOWNORMAL);
  UpdateWindow(window1->wnd);

  *window = window1;

  return 0;
}

int dx_gl_wgl_open_window() {
  dx_gl_wgl_window* window_v1;
  if (dx_gl_wgl_open_window_internal(&window_v1, NULL, &dx_gl_wgl_init_wgl_v1)) {
    return 1;
  }
  dx_gl_wgl_window* window_v2;
  if (dx_gl_wgl_open_window_internal(&window_v2, window_v1, &dx_gl_wgl_init_wgl_v2)) {
    return 1;
  }

  dx_gl_wgl_close_window_internal(window_v1);
  
  if (!wglMakeCurrent(window_v2->dc, window_v2->glrc)) {
    dx_log("unable to make wgl context current\n", sizeof("unable to make wgl context current\n"));
    dx_gl_wgl_close_window_internal(window_v2);
    window_v2 = NULL;
    return 1;
  }

  g_window = window_v2;

  return 0;
}

int dx_gl_wgl_close_window() {
  dx_gl_wgl_close_window_internal(g_window);
  g_window = NULL;
#if 0
  if (g_window->glrc) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(g_window->glrc);
    g_window->glrc = NULL;
  }
  if (g_window->dc) {
    ReleaseDC(g_window->wnd, g_window->dc);
    g_window->dc = NULL;
  }
  if (g_window->wnd) {
    DestroyWindow(g_window->wnd);
    g_window->wnd = NULL;
  }
#endif
  return 0;
}

static int dx_gl_wgl_init_wgl_v2(dx_gl_wgl_window* window, dx_gl_wgl_window* existing) {
  if (!wglMakeCurrent(existing->dc, existing->glrc)) {
    dx_log("unable to make wgl context current\n", sizeof("unable to make wgl context current\n"));
    return 1;
  }
  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
  if (!wglChoosePixelFormat) {
    return 1;
  }
  const int samples = 0;
  const int pixel_format_attribs[] = {
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 24,
    WGL_ALPHA_BITS_ARB, 8,
    WGL_DEPTH_BITS_ARB, 24,
    WGL_STENCIL_BITS_ARB, 8,
    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    WGL_SAMPLE_BUFFERS_ARB, samples > 0 ? 1 : 0,
    WGL_SAMPLES_ARB, samples,
    WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
    0, 0
  };
  {
    int i, n;
    if (!wglChoosePixelFormat(window->dc, pixel_format_attribs, 0, 1, &i, &n)) {
      dx_log("unable to choose pixel format\n", sizeof("unable to choose pixel format\n"));
      return 1;
    }
    PIXELFORMATDESCRIPTOR desc;
    if (!DescribePixelFormat(window->dc, i, sizeof(desc), &desc)) {
      dx_log("unable to describe pixel format\n", sizeof("unable to describe pixel format\n"));
      return 1;
    }
    if (!SetPixelFormat(window->dc, i, &desc)) {
      dx_log("unable to set pixel format\n", sizeof("unable to set pixel format\n"));
      return 1;
    }
  }

  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
  if (!wglCreateContextAttribs) {
    return 1;
  }
  const int context_attribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 1,
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };
  window->glrc = wglCreateContextAttribs(window->dc, NULL, context_attribs);
  if (!window->glrc) {
    dx_log("unable to create wgl context\n", sizeof("unable to create wgl context\n"));
    return 1;
  }

  return 0;
}

static int dx_gl_wgl_init_wgl_v1(dx_gl_wgl_window* window, dx_gl_wgl_window* existing) {
  PIXELFORMATDESCRIPTOR desc = {
    .nSize = sizeof(desc),
    .nVersion = 1,
    .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    .iPixelType = PFD_TYPE_RGBA,
    .cColorBits = 24,
  };
  int format = ChoosePixelFormat(window->dc, &desc);
  if (!format) {
    dx_log("unable to choose pixel format\n", sizeof("unable to choose pixel format\n"));
    return 1;
  }
  if (!DescribePixelFormat(window->dc, format, sizeof(desc), &desc)) {
    dx_log("unable to describe pixel format\n", sizeof("unable to describe pixel format\n"));
    return 1;
  }
  if (!SetPixelFormat(window->dc, format, &desc)) {
    dx_log("unable to set pixel format\n", sizeof("unable to set pixel format\n"));
    return 1;
  }
  window->glrc = wglCreateContext(window->dc);
  if (!window->glrc) {
    dx_log("unable to create wgl context\n", sizeof("unable to create wgl context\n"));
    return 1;
  }
  return 0;
}

static dx_gl_wgl_context* g_context = NULL;

int dx_gl_wgl_open_wm() {
  if (dx_gl_wgl_open_window()) {
    return 1;
  }
  g_context = dx_gl_wgl_context_create();
  if (!g_context) {
    dx_gl_wgl_close_window();
    return 1;
  }
  return 0;
}

int dx_gl_wgl_close_wm() {
  DX_UNREFERENCE(g_context);
  g_context = NULL;
  dx_gl_wgl_close_window();
  return 0;
}

int dx_gl_wgl_get_canvas_size(int* w, int* h) {
  if (!w || !h) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  RECT rect;
  GetClientRect(g_window->wnd, &rect);
  *w = rect.right - rect.left;
  *h = rect.bottom - rect.top;
  return 0;
}

dx_gl_wgl_context* dx_gl_wgl_get_context() {
  return g_context;
}

void dx_gl_wgl_enter_frame() {

}

void dx_gl_wgl_leave_frame() {
  SwapBuffers(g_window->dc);
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/