#include "dx/gl/wgl/wm.h"

#include "dx/core.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "opengl32.lib")

#include "dx/gl/wgl/context.h"

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: quit message emission is traced.
/// - @a 0: quit message emission is not traced.
#define DX_QUIT_MSG_TRACE (0)

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: keyboard key message emission is traced.
/// - @a 0: keyboard key message emission is not traced.
#define DX_KEYBOARD_KEY_MSG_TRACE (0)

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: mouse button message emission is traced.
/// - @a 0: mouse button message emission is not traced.
#define DX_MOUSE_BUTTON_MSG_TRACE (0)

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: mouse pointer message emission is traced.
/// - @a 0: mouse pointer message emission is not traced.
#define DX_MOUSE_POINTER_MSG_TRACE (0)

/// @brief
/// Must be defined to either @a 1 or @a 0.
/// - @a 1: canvas message emission is traced.
/// - @a 0: canvas message emission is not traced.
#define DX_CANVAS_MSG_TRACE (0)

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

static int emit_keyboard_key_pressed_msg(dx_keyboard_key key);

static int emit_keyboard_key_released_msg(dx_keyboard_key key);

static int emit_mouse_button_pressed_msg(dx_mouse_button button, dx_f32 x, dx_f32 y);

static int emit_mouse_button_released_msg(dx_mouse_button button, dx_f32 x, dx_f32 y);

static int emit_mouse_pointer_moved_msg(dx_f32 x, dx_f32 y);

static int emit_canvas_size_changed_msg(dx_f32 width, dx_f32 height);

static int emit_canvas_activated_msg();

static int emit_canvas_deactivated_msg();

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

#if !defined(DX_QUIT_MSG_TRACE) || ((DX_QUIT_MSG_TRACE != 1) && (DX_QUIT_MSG_TRACE != 0))
  #error("DX_QUIT_MSG_TRACE must be defined to 1 or 0")
#endif
#if 1 == DX_QUIT_MSG_TRACE
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

static int emit_quit_msg() {
  ENTER(__func__);
  // create the "quit" message.
  dx_msg* msg = DX_MSG(dx_quit_msg_create());
  if (!msg) {
    LEAVE(__func__);
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    LEAVE(__func__);
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  LEAVE(__func__);
  return 0;
}

#undef TRACE

#if !defined(DX_KEYBOARD_KEY_MSG_TRACE) || ((DX_KEYBOARD_KEY_MSG_TRACE != 1) && (DX_KEYBOARD_KEY_MSG_TRACE != 0))
  #error("DX_KEYBOARD_KEY_MSG_TRACE must be defined to 1 or 0")
#endif
#if 1 == DX_KEYBOARD_KEY_MSG_TRACE
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

static int emit_keyboard_key_pressed_msg(dx_keyboard_key key) {
  ENTER(__func__);
  // create the "keyboard key" message.
  uint8_t modifiers = 0;
  //
  if (GetKeyState(VK_LCONTROL)) {
    modifiers |= dx_modifier_lctrl;
  }
  if (GetKeyState(VK_RCONTROL)) {
    modifiers |= dx_modifier_rctrl;
  }
  //
  if (GetKeyState(VK_LSHIFT)) {
    modifiers |= dx_modifier_lshift;
  }
  if (GetKeyState(VK_RSHIFT)) {
    modifiers |= dx_modifier_rshift;
  }
  //
  if (GetKeyState(VK_LMENU)) {
    modifiers |= dx_modifier_lmenu;
  }
  if (GetKeyState(VK_RMENU)) {
    modifiers |= dx_modifier_rmenu;
  }
  dx_msg* msg = DX_MSG(dx_keyboard_key_msg_create(DX_KEYBOARD_KEY_ACTION_PRESSED, key, modifiers));
  if (!msg) {
    LEAVE(__func__);
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    LEAVE(__func__);
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  LEAVE(__func__);
  return 0;
}

static int emit_keyboard_key_released_msg(dx_keyboard_key key) {
  ENTER(__func__);
  // create the "keyboard key" message.
  uint8_t modifiers = 0;
  //
  if (GetKeyState(VK_LCONTROL)) {
    modifiers |= dx_modifier_lctrl;
  }
  if (GetKeyState(VK_RCONTROL)) {
    modifiers |= dx_modifier_rctrl;
  }
  //
  if (GetKeyState(VK_LSHIFT)) {
    modifiers |= dx_modifier_lshift;
  }
  if (GetKeyState(VK_RSHIFT)) {
    modifiers |= dx_modifier_rshift;
  }
  //
  if (GetKeyState(VK_LMENU)) {
    modifiers |= dx_modifier_lmenu;
  }
  if (GetKeyState(VK_RMENU)) {
    modifiers |= dx_modifier_rmenu;
  }
  dx_msg* msg = DX_MSG(dx_keyboard_key_msg_create(DX_KEYBOARD_KEY_ACTION_RELEASED, key, modifiers));
  if (!msg) {
    LEAVE(__func__);
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    LEAVE(__func__);
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  LEAVE(__func__);
  return 0;
}

#undef TRACE

#if !defined(DX_MOUSE_BUTTON_MSG_TRACE) || ((DX_MOUSE_BUTTON_MSG_TRACE != 1) && (DX_MOUSE_BUTTON_MSG_TRACE != 0))
  #error("DX_MOUSE_BUTTON_MSG_TRACE must be defined to 1 or 0")
#endif
#if 1 == DX_MOUSE_BUTTON_MSG_TRACE
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

static int emit_mouse_button_pressed_msg(dx_mouse_button button, dx_f32 x, dx_f32 y) {
  ENTER(__func__);
  // create the "mouse button" message.
  uint8_t modifiers = 0;
  //
  if (GetKeyState(VK_LCONTROL)) {
    modifiers |= dx_modifier_lctrl;
  }
  if (GetKeyState(VK_RCONTROL)) {
    modifiers |= dx_modifier_rctrl;
  }
  //
  if (GetKeyState(VK_LSHIFT)) {
    modifiers |= dx_modifier_lshift;
  }
  if (GetKeyState(VK_RSHIFT)) {
    modifiers |= dx_modifier_rshift;
  }
  //
  if (GetKeyState(VK_LMENU)) {
    modifiers |= dx_modifier_lmenu;
  }
  if (GetKeyState(VK_RMENU)) {
    modifiers |= dx_modifier_rmenu;
  }
  dx_msg* msg = DX_MSG(dx_mouse_button_msg_create(DX_MOUSE_BUTTON_ACTION_PRESSED, button, modifiers, x, y));
  if (!msg) {
    LEAVE(__func__);
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    LEAVE(__func__);
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  LEAVE(__func__);
  return 0;
}

static int emit_mouse_button_released_msg(dx_mouse_button button, dx_f32 x, dx_f32 y) {
  ENTER(__func__);
  // create the "mouse button" message.
  uint8_t modifiers = 0;
  //
  if (GetKeyState(VK_LCONTROL)) {
    modifiers |= dx_modifier_lctrl;
  }
  if (GetKeyState(VK_RCONTROL)) {
    modifiers |= dx_modifier_rctrl;
  }
  //
  if (GetKeyState(VK_LSHIFT)) {
    modifiers |= dx_modifier_lshift;
  }
  if (GetKeyState(VK_RSHIFT)) {
    modifiers |= dx_modifier_rshift;
  }
  //
  if (GetKeyState(VK_LMENU)) {
    modifiers |= dx_modifier_lmenu;
  }
  if (GetKeyState(VK_RMENU)) {
    modifiers |= dx_modifier_rmenu;
  }
  dx_msg* msg = DX_MSG(dx_mouse_button_msg_create(DX_MOUSE_BUTTON_ACTION_RELEASED, button, modifiers, x, y));
  if (!msg) {
    LEAVE(__func__);
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    LEAVE(__func__);
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  LEAVE(__func__);
  return 0;
}

#undef TRACE

#if !defined(DX_MOUSE_POINTER_MSG_TRACE) || ((DX_MOUSE_POINTER_MSG_TRACE != 1) && (DX_MOUSE_POINTER_MSG_TRACE != 0))
  #error("DX_MOUSE_POINTER_MSG_TRACE must be defined to 1 or 0")
#endif
#if 1 == DX_MOUSE_POINTER_MSG_TRACE
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

static int emit_mouse_pointer_moved_msg(dx_f32 x, dx_f32 y) {
  ENTER(__func__);
  // create the "mouse pointer" message.
  uint8_t modifiers = 0;
  //
  if (GetKeyState(VK_LCONTROL)) {
    modifiers |= dx_modifier_lctrl;
  }
  if (GetKeyState(VK_RCONTROL)) {
    modifiers |= dx_modifier_rctrl;
  }
  //
  if (GetKeyState(VK_LSHIFT)) {
    modifiers |= dx_modifier_lshift;
  }
  if (GetKeyState(VK_RSHIFT)) {
    modifiers |= dx_modifier_rshift;
  }
  //
  if (GetKeyState(VK_LMENU)) {
    modifiers |= dx_modifier_lmenu;
  }
  if (GetKeyState(VK_RMENU)) {
    modifiers |= dx_modifier_rmenu;
  }
  dx_msg* msg = DX_MSG(dx_mouse_pointer_msg_create(DX_MOUSE_POINTER_ACTION_MOVED, modifiers, x, y));
  if (!msg) {
    LEAVE(__func__);
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    LEAVE(__func__);
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  LEAVE(__func__);
  return 0;
}

#undef TRACE

#if !defined(DX_CANVAS_MSG_TRACE) || ((DX_CANVAS_MSG_TRACE != 1) && (DX_CANVAS_MSG_TRACE != 0))
  #error("DX_CANVAS_MSG_TRACE must be defined to 1 or 0")
#endif
#if 1 == DX_CANVAS_MSG_TRACE
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

static int emit_canvas_size_changed_msg(dx_f32 width, dx_f32 height) {
  ENTER(__func__);
  dx_msg* msg = DX_MSG(dx_canvas_size_changed_msg_create(width, height));
  if (!msg) {
    LEAVE(__func__);
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    LEAVE(__func__);
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  LEAVE(__func__);
  return 0;
}

static int emit_canvas_activated_msg() {
  ENTER(__func__);
  dx_msg* msg = DX_MSG(dx_canvas_msg_create(dx_canvas_msg_kind_activated));
  if (!msg) {
    LEAVE(__func__);
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    LEAVE(__func__);
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  LEAVE(__func__);
  return 0;
}

static int emit_canvas_deactivated_msg() {
  ENTER(__func__);
  dx_msg* msg = DX_MSG(dx_canvas_msg_create(dx_canvas_msg_kind_deactivated));
  if (!msg) {
    LEAVE(__func__);
    return 1;
  }
  if (dx_msg_queue_push(g_application->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    LEAVE(__func__);
    return 1;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  LEAVE(__func__);
  return 0;
}

static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    // keyboard keys
    case WM_KEYUP: {
      if (wparam == VK_ESCAPE) {
        emit_quit_msg();
      } else if (wparam == VK_RETURN) {
        emit_keyboard_key_released_msg(dx_keyboard_key_return);
      }
      return 0;
    } break;
    case WM_KEYDOWN: {
      return 0;
    } break;
    // left mouse button
    case WM_LBUTTONDOWN: {
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      emit_mouse_button_pressed_msg(dx_mouse_button_button_0, x, y);
      return 0;
    } break;
    case WM_LBUTTONUP: {
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      // When the window loses focus, windows might not send the button up message.
      // Most likely, a mouse leave message is sent instead by windows.
      emit_mouse_button_released_msg(dx_mouse_button_button_0, x, y );
      return 0;
    } break;
    // middle mouse button
    case WM_MBUTTONDOWN: {
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      emit_mouse_button_pressed_msg(dx_mouse_button_button_1, x, y);
      return 0;
    } break;
    case WM_MBUTTONUP: {
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      // When the window loses focus, windows might not send the button up message.
      // Most likely, a mouse leave message is sent instead by windows.
      emit_mouse_button_released_msg(dx_mouse_button_button_1, x, y);
      return 0;
    } break;
    // right mouse button
    case WM_RBUTTONDOWN: {
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      // When the window loses focus, windows might not send the button up message.
      // Most likely, a mouse leave message is sent instead by windows.
      emit_mouse_button_pressed_msg(dx_mouse_button_button_2, x, y);
      return 0;
    } break;
    case WM_RBUTTONUP: {
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      // When the window loses focus, windows might not send the button up message.
      // Most likely, a mouse leave message is sent instead by windows.
      emit_mouse_button_released_msg(dx_mouse_button_button_2, x, y);
      return 0;
    } break;
    // mouse pointer
    case WM_MOUSEMOVE: {
      dx_f32 x = (dx_f32)(int)GET_X_LPARAM(lparam);
      dx_f32 y = (dx_f32)(int)GET_Y_LPARAM(lparam);
      emit_mouse_pointer_moved_msg(x, y);
      return 0;
    } break;
    // application/window
    case WM_SIZE: {
      UINT width = LOWORD(lparam);
      UINT height = HIWORD(lparam);
      emit_canvas_size_changed_msg((dx_f32)width, (dx_f32)height);
      return 0;
    } break;
    case WM_ACTIVATE: {
      if (wparam) {
        // canvas was activated
        emit_canvas_activated_msg();
      } else {
        // canvas was deactivated
        emit_canvas_deactivated_msg();
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

DX_DEFINE_OBJECT_TYPE("dx.gl.wgl.application",
                      dx_gl_wgl_application,
                      dx_application)

int dx_gl_wgl_application_construct(dx_gl_wgl_application* self, dx_msg_queue* msg_queue) {
  dx_rti_type* _type = dx_gl_wgl_application_get_type();
  if (!_type) {
    return 1;
  }
  //
  if (dx_application_construct(DX_APPLICATION(self))) {
    return 1;
  }
  //
  self->msg_queue = msg_queue;

  //
  self->class_name = _strdup("dx.gl.wgl.window.class_name");
  if (!self->class_name) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }

  //
  self->instance_handle = GetModuleHandle(NULL);
  if (!self->instance_handle) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    dx_memory_deallocate(self->class_name);
    self->class_name = NULL;
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
  wcex.hInstance = self->instance_handle;
  wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = self->class_name;
  wcex.hIconSm = NULL;
  self->class_handle = RegisterClassEx(&wcex);
  if (!self->class_handle) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    self->instance_handle = 0;
    dx_memory_deallocate(self->class_name);
    self->class_name = NULL;
    dx_log("unable to register window class\n", sizeof("unable to register window class\n"));
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_gl_wgl_application_destruct(dx_gl_wgl_application* self) {
  if (self->class_handle) {
    if (!UnregisterClass(self->class_name, self->instance_handle)) {
      dx_log("unable to unregister window class\n", sizeof("unable to unregister window class\n"));
    }
    self->class_handle = 0;
  }
  if (self->instance_handle) {
    self->instance_handle = 0;
  }
  if (self->class_name) {
    dx_memory_deallocate(self->class_name);
    self->class_name = NULL;
  }
}

dx_gl_wgl_application* dx_gl_wgl_application_create(dx_msg_queue* msg_queue) {
  dx_gl_wgl_application* self = DX_GL_WGL_APPLICATION(dx_object_alloc(sizeof(dx_gl_wgl_application)));
  if (!self) {
    return NULL;
  }
  if (dx_gl_wgl_application_construct(self, msg_queue)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
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
  dx_memory_deallocate(window);
}

static int dx_gl_wgl_open_window_internal(dx_gl_wgl_window** window, dx_gl_wgl_window* existing, int (*init_wgl)(dx_gl_wgl_window*, dx_gl_wgl_window*)) {
  if (!g_application) {
    return 1;
  }
  dx_gl_wgl_window* window1 = dx_memory_allocate(sizeof(dx_gl_wgl_window));
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
    dx_memory_deallocate(window1);
    window1 = NULL;
    return 1;
  }
  window1->dc = GetDC(window1->wnd);
  if (!window1->dc) {
    dx_log("unable to create drawing context\n", sizeof("unable to create drawing context\n"));
    DestroyWindow(window1->wnd);
    window1->wnd = NULL;
    dx_memory_deallocate(window1);
    window1 = NULL;
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
  // If an error is pending, then this function fails.
  return DX_NO_ERROR != dx_get_error();
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