#include "dx/gl/wgl/wm.h"

// malloc, free
#include <malloc.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "opengl32.lib")

#include "dx/gl/wgl/context.h"
#include "dx/input_msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct dx_gl_wgl_window dx_gl_wgl_window;

struct dx_gl_wgl_window {
  HWND wnd;
  HDC dc;
  HGLRC glrc;
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

static dx_gl_wgl_application* g_application = NULL;

static int emit_quit_msg();

static int emit_keyboard_key_pressed_msg(int key);

static int emit_keyboard_key_released_msg(int key);

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

static int emit_quit_msg() {
  dx_log("enter: emit_quit_msg\n", sizeof("enter: emit_quit_msg\n"));
  // create the "quit" message.
  dx_msg* msg = DX_MSG(dx_quit_msg_create());
  if (!msg) {
    dx_log("leave: emit_quit_msg\n", sizeof("leave: emit_quit_msg\n"));
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    dx_log("leave: emit_quit_msg\n", sizeof("leave: emit_quit_msg\n"));
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  dx_log("leave: emit_quit_msg\n", sizeof("leave: emit_quit_msg\n"));
  return 0;
}

static int emit_keyboard_key_pressed_msg(int key) {
  dx_log("enter: emit_keyboard_key_pressed_msg\n", sizeof("enter: emit_keyboard_key_pressed_msg\n"));
  // create the "keyboard key" message.
  dx_msg* msg = DX_MSG(dx_keyboard_key_msg_create(DX_KEYBOARD_KEY_ACTION_PRESSED, DX_KEYBOARD_KEY_ENTER));
  if (!msg) {
    dx_log("leave: emit_keyboard_key_pressed_msg\n", sizeof("leave: emit_keyboard_key_pressed_msg\n"));
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    dx_log("leave: emit_keyboard_key_pressed_msg\n", sizeof("leave: emit_keyboard_key_pressed_msg\n"));
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  dx_log("leave: emit_keyboard_key_pressed_msg\n", sizeof("leave: emit_keyboard_key_pressed_msg\n"));
  return 0;
}

static int emit_keyboard_key_released_msg(int key) {
  dx_log("enter: emit_keyboard_key_released_msg\n", sizeof("enter: emit_keyboard_key_released_msg\n"));
  // create the "keyboard key" message.
  dx_msg* msg = DX_MSG(dx_keyboard_key_msg_create(DX_KEYBOARD_KEY_ACTION_RELEASED, DX_KEYBOARD_KEY_ENTER));
  if (!msg) {
    dx_log("leave: emit_keyboard_key_released_msg\n", sizeof("leave: emit_keyboard_key_released_msg\n"));
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    dx_log("leave: emit_keyboard_key_released_msg\n", sizeof("leave: emit_keyboard_key_released_msg\n"));
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  dx_log("leave: emit_keyboard_key_released_msg\n", sizeof("leave: emit_keyboard_key_released_msg\n"));
  return 0;
}

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    case WM_KEYUP: {
      if (wparam == VK_ESCAPE) {
        emit_quit_msg();
      } else if (wparam == VK_RETURN) {
        emit_keyboard_key_released_msg(DX_KEYBOARD_KEY_ENTER);
      }
      return 0;
    } break;
    case WM_CLOSE: {
      emit_quit_msg();
      return 0;
    } break;
    default: {
      return DefWindowProc(wnd, msg, wparam, lparam);
    } break;
  };
}

int dx_gl_wgl_application_construct(dx_gl_wgl_application* application, dx_msg_queue* msg_queue) {
  //
  if (dx_application_construct(DX_APPLICATION(application))) {
    return 1;
  }
  //
  application->msg_queue = msg_queue;

  //
  application->class_name = _strdup("dx.gl.wgl.window.class_name");
  if (!application->class_name) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }

  //
  application->instance_handle = GetModuleHandle(NULL);
  if (!application->instance_handle) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    free(application->class_name);
    application->class_name = NULL;
    dx_log("unable to acquire module handle\n", sizeof("unable to acquire module handle\n"));
    return 1;
  }

  //
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wcex.lpfnWndProc = &window_procedure;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = application->instance_handle;
  wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = application->class_name;
  wcex.hIconSm = NULL;
  application->class_handle = RegisterClassEx(&wcex);
  if (!application->class_handle) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    application->instance_handle = 0;
    free(application->class_name);
    application->class_name = NULL;
    dx_log("unable to register window class\n", sizeof("unable to register window class\n"));
    return 1;
  }
  DX_OBJECT(application)->destruct = (void(*)(dx_object*)) & dx_gl_wgl_application_destruct;
  return 0;
}

void dx_gl_wgl_application_destruct(dx_gl_wgl_application* application) {
  if (application->class_handle) {
    if (!UnregisterClass(application->class_name, application->instance_handle)) {
      dx_log("unable to unregister window class\n", sizeof("unable to unregister window class\n"));
    }
    g_application->class_handle = 0;
  }
  if (application->instance_handle) {
    application->instance_handle = 0;
  }
  if (application->class_name) {
    free(application->class_name);
    application->class_name = NULL;
  }
  dx_application_destruct(DX_APPLICATION(application));
}

dx_gl_wgl_application* dx_gl_wgl_application_create(dx_msg_queue* msg_queue) {
  dx_gl_wgl_application* application = DX_GL_WGL_APPLICATION(dx_object_alloc(sizeof(dx_gl_wgl_application)));
  if (!application) {
    return NULL;
  }
  if (dx_gl_wgl_application_construct(application, msg_queue)) {
    DX_UNREFERENCE(application);
    application = NULL;
    return NULL;
  }
  return application;
}

int dx_gl_wgl_application_startup(dx_msg_queue* msg_queue) {
  if (g_application) {
    return 1;
  }
  g_application = dx_gl_wgl_application_create(msg_queue);
  if (!g_application) {
    return 1;
  }
  return 0; 
}

void dx_gl_wgl_application_shutdown() {
  if (g_application) {
    DX_UNREFERENCE(g_application);
    g_application = NULL;
  }
}

dx_gl_wgl_application* dx_gl_wgl_application_get() {
  if (!g_application) {
    return NULL;
  }
  DX_REFERENCE(g_application);
  return g_application;
}

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
}

static int dx_gl_wgl_open_window_internal(dx_gl_wgl_window** window, dx_gl_wgl_window* existing, int (*init_wgl)(dx_gl_wgl_window*, dx_gl_wgl_window*)) {
  if (!g_application) {
    return 1;
  }
  dx_gl_wgl_window* window1 = malloc(sizeof(dx_gl_wgl_window));
  if (!window1) {
    dx_log("allocation failed\n", sizeof("allocation failed\n"));
    return 1;
  }
  window1->wnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                                g_application->class_name,
                                "Media Evaluation Framework",
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                NULL,
                                NULL,
                                g_application->instance_handle,
                                NULL);
  if (!window1->wnd) {
    dx_log("unable to create window\n", sizeof("unable to create window\n"));
    free(window1);
    window1 = NULL;
    return 1;
  }
  window1->dc = GetDC(window1->wnd);
  if (!window1->dc) {
    dx_log("unable to create drawing context\n", sizeof("unable to create drawing context\n"));
    DestroyWindow(window1->wnd);
    window1->wnd = NULL;
    free(window1);
    window1 = NULL;
    return 1;
  }

  if (init_wgl) {
    if (init_wgl(window1, existing)) {
      ReleaseDC(window1->wnd, window1->dc);
      window1->dc = NULL;
      DestroyWindow(window1->wnd);
      window1->wnd = NULL;
      free(window1);
      window1 = NULL;
      return 1;
    }
  }

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

int dx_gl_wgl_update_wm() {
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      emit_quit_msg();
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);  
    }
  }
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