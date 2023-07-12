#include "dx/gl/wgl/application.h"

// GET_X_LPARAM, GET_Y_LPARAM
#include <windowsx.h>

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

int update(dx_gl_wgl_application* application);

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
  if (dx_msg_queue_push(DX_GL_APPLICATION(g_application)->msg_queue, msg)) {
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
  if (dx_msg_queue_push(DX_GL_APPLICATION(g_application)->msg_queue, msg)) {
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
  if (dx_msg_queue_push(DX_GL_APPLICATION(g_application)->msg_queue, msg)) {
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
  if (dx_msg_queue_push(DX_GL_APPLICATION(g_application)->msg_queue, msg)) {
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
  if (dx_msg_queue_push(DX_GL_APPLICATION(g_application)->msg_queue, msg)) {
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
  if (dx_msg_queue_push(DX_GL_APPLICATION(g_application)->msg_queue, msg)) {
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
  if (dx_msg_queue_push(DX_GL_APPLICATION(g_application)->msg_queue, msg)) {
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
  if (dx_msg_queue_push(DX_GL_APPLICATION(g_application)->msg_queue, msg)) {
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
  if (dx_msg_queue_push(DX_GL_APPLICATION(g_application)->msg_queue, msg)) {
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

static int update(dx_gl_wgl_application* application) {
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.gl.wgl.application",
                      dx_gl_wgl_application,
                      dx_gl_application)

int dx_gl_wgl_application_construct(dx_gl_wgl_application* self, dx_msg_queue* msg_queue) {
  dx_rti_type* _type = dx_gl_wgl_application_get_type();
  if (!_type) {
    return 1;
  }
  //
  if (dx_gl_application_construct(DX_GL_APPLICATION(self), msg_queue)) {
    return 1;
  }
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
  DX_APPLICATION(self)->update = (int(*)(dx_application*)) & update;
  DX_OBJECT(self)->type = _type;
  /// >>>
  g_application = self;
  /// <<<
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
  // >>>
  g_application = NULL;
  // <<<
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

dx_gl_wgl_application* dx_gl_wgl_application_get() {
  if (!g_application) {
    return NULL;
  }
  DX_REFERENCE(g_application);
  return g_application;
}
