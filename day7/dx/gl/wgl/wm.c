#include "dx/gl/wgl/wm.h"

// malloc, free
#include <malloc.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct dx_gl_wgl_window dx_gl_wgl_window;

struct dx_gl_wgl_window {
  HWND wnd;
  HDC dc;
};

static int dx_gl_wgl_open_window_internal(dx_gl_wgl_window** window, dx_gl_wgl_window* existing);

static void dx_gl_wgl_close_window_internal(dx_gl_wgl_window* window);

int dx_gl_wgl_open_window();

int dx_gl_wgl_close_window();

static dx_gl_wgl_window* g_window = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_gl_wgl_application* g_application = NULL;

static int emit_quit_msg();

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

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    case WM_KEYUP: {
      if (wparam == VK_ESCAPE) {
        emit_quit_msg();
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
  if (window->dc) {
    ReleaseDC(window->wnd, window->dc);
    window->dc = NULL;
  }
  if (window->wnd) {
    DestroyWindow(window->wnd);
    window->wnd = NULL;
  }
}

static int dx_gl_wgl_open_window_internal(dx_gl_wgl_window** window, dx_gl_wgl_window* existing) {
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

  ShowWindow(window1->wnd, SW_SHOWNORMAL);
  UpdateWindow(window1->wnd);

  *window = window1;

  return 0;
}

int dx_gl_wgl_open_window() {
  dx_gl_wgl_window* window;
  if (dx_gl_wgl_open_window_internal(&window, NULL)) {
    return 1;
  }
  g_window = window;
  return 0;
}

int dx_gl_wgl_close_window() {
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

int dx_gl_wgl_open_wm() {
  if (dx_gl_wgl_open_window()) {
    return 1;
  }
  return 0;
}

int dx_gl_wgl_close_wm() {
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
