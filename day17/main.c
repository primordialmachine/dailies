// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

#include "dx/core.h"

// GetTickCount64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#if defined(_WIN32)
  #include "dx/gl/wgl/wm.h"
#else
  #error("environment not (yet) supported")
#endif

#define DX_TRACE (0)

#if !defined(DX_TRACE) || (1 != DX_TRACE && 0 != DX_TRACE)
  #error("DX_TRACE must be defined to 1 or 0")
#endif
#if 1 == DX_TRACE
  #define ENTER(FUNCTION_NAME) dx_log("enter `", sizeof("enter `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
  #define LEAVE(FUNCTION_NAME) dx_log("leave `", sizeof("leave `") - 1); dx_log(FUNCTION_NAME, strlen(FUNCTION_NAME)); dx_log("`\n", sizeof("`\n") - 1);
#else
  #define ENTER(FUNCTION_NAME)
  #define LEAVE(FUNCTION_NAME)
#endif

#include "dx/application.h"
#include "dx/gl/wgl/wm.h"
#include "dx/scenes/mesh_viewer_scene.h"
#include "dx/fps_counter.h"
#include <stdio.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static char const* PATHNAMES[] = {
  "./assets/day16-triangle.adl",
  "./assets/day16-quadriliteral.adl",
  "./assets/day17-cube.adl",
  "./assets/day17-octahedron.adl",
  "./assets/day18-cube.adl",
  "./assets/day18-octahedron.adl"
};

static dx_size const NUMBER_OF_PATHNAMES = sizeof(PATHNAMES) / sizeof(char const*);

static bool g_quit = false;

static dx_fps_counter* g_fps_counter = NULL;

static dx_msg_queue *g_msg_queue  = NULL;

static dx_size g_scene_index = 0;

static dx_scene* g_scenes[6] = { NULL, NULL, NULL, NULL, NULL, NULL, };

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int on_msg(dx_msg* msg);

static int on_render_scene(dx_context* context, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height);

static int run();

static int startup();

static int shutdown();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_application* g_application = NULL;

/// @brief Create the singleton application object with an initial reference count of @a 1.
/// For a successful invocation of this function, the caller must invoke dx_application_shutdown.
/// @param msq_queue Pointer to the message queue object the application shall use.
/// @return A zero value on success. A non-zero value on failure.
/// @warning
/// This function will fail
/// - if @a msg_queue is a null pointer or
//  - if the application is already started up.
int dx_application_startup(dx_msg_queue* msg_queue);

/// @brief Decrement the reference count of the singleton application object by @a 1.
/// @remarks
/// Try to shutdown the application.
/// The application will not shut down until all references to the dx_application object are released.
void dx_application_shutdown();

/// @brief Get the singleton application object.
/// @return Returns null if the application does not exist.
/// Otherwise a pointer to the application object is returned an the reference count of the application object is incremented by @a 1.
dx_application* dx_application_get();

int dx_application_startup(dx_msg_queue* msg_queue) {
  g_application = DX_APPLICATION(dx_gl_wgl_application_create(msg_queue));
  if (!g_application) {
    return 1;
  }
  return 0;
}

void dx_application_shutdown() {
  DX_UNREFERENCE(g_application);
  g_application = NULL;
}

dx_application* dx_application_get() {
  DX_REFERENCE(g_application);
  return g_application;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int on_msg(dx_msg* msg) {
  switch (dx_msg_get_flags(msg)) {
  case DX_MSG_TYPE_EMIT: {
    dx_emit_msg* emit_msg = DX_EMIT_MSG(msg);
    char const* p; dx_size n;
    if (dx_emit_msg_get(emit_msg, &p, &n)) {
      return 1;
    }
    dx_log(p, n);
  } break;
  case DX_MSG_TYPE_INPUT: {
    dx_input_msg* input_msg = DX_INPUT_MSG(msg);
    if (DX_INPUT_MSG_KIND_KEYBOARD_KEY == dx_input_msg_get_kind(input_msg)) {
      dx_keyboard_key_msg* keyboard_key_msg = DX_KEYBOARD_KEY_MSG(input_msg);
      if (DX_KEYBOARD_KEY_ACTION_RELEASED == dx_keyboard_key_msg_get_action(keyboard_key_msg) && dx_keyboard_key_return == dx_keyboard_key_msg_get_key(keyboard_key_msg)) {
        g_scene_index = (g_scene_index + 1) % NUMBER_OF_PATHNAMES;
      }
    };
  } break;
  case DX_MSG_TYPE_QUIT: {
    g_quit = true;
  } break;
  case DX_MSG_TYPE_UNDETERMINED:
  default: {
    return 0;
  } break;
  }
  return 0;
}

static int on_startup_scene(dx_context* context) {
  //
  for (dx_size i = 0, n = NUMBER_OF_PATHNAMES; i < n; ++i) {
    g_scenes[i] = DX_SCENE(dx_mesh_viewer_scene_create(PATHNAMES[i]));
    if (!g_scenes[i]) {
      while (i > 0) {
        DX_UNREFERENCE(g_scenes[--i]);
        g_scenes[i] = NULL;
      }
      return 1;
    }
  }
  //
  for (dx_size i = 0, n = NUMBER_OF_PATHNAMES; i < n; ++i) {
    if (dx_scene_startup(g_scenes[i], context)) {
      while (i > 0) {
        dx_scene_shutdown(g_scenes[--i], context);
      }
      for (dx_size i = NUMBER_OF_PATHNAMES; i > 0; --i) {
        DX_UNREFERENCE(g_scenes[i - 1]);
        g_scenes[i - 1] = NULL;
      }
    }
  }
  //
  return 0;
}

static int on_shutdown_scene(dx_context* context) {
  for (dx_size i = NUMBER_OF_PATHNAMES; i > 0; --i) {
    dx_scene_shutdown(g_scenes[i - 1], context);
    DX_UNREFERENCE(g_scenes[i - 1]);
    g_scenes[i - 1] = NULL;
  }
  return 0;
}

static int on_render_scene(dx_context* context, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height) {
  if (dx_scene_render(g_scenes[g_scene_index], context, delta_seconds, canvas_width, canvas_height)) {
    return 1;
  }
  return 0;
}

static int run() {
  ENTER(DX_C_FUNCTION_NAME);
  {
    dx_msg* msg;
    
    // create the "emit" message.
    msg = DX_MSG(dx_emit_msg_create("Hello, World!\n", sizeof("Hello, World!\n")));
    if (!msg) {
      LEAVE(DX_C_FUNCTION_NAME);
      return 1;
    }
    if (dx_msg_queue_push(g_msg_queue, msg)) {
      DX_UNREFERENCE(msg);
      msg = NULL;
      LEAVE(DX_C_FUNCTION_NAME);
      return 1;
    }
    DX_UNREFERENCE(msg);
    msg = NULL;
  }
  
  dx_context* ctx = DX_CONTEXT(dx_gl_wgl_get_context());
  
  if (on_startup_scene(ctx)) {
    LEAVE(DX_C_FUNCTION_NAME);
    return 1;
  }
  
  uint64_t last = GetTickCount64();
  uint64_t now = last;
  uint64_t delta = now - last;
  
  while (!g_quit) {
    dx_fps_counter_on_enter_frame(g_fps_counter);

    now = GetTickCount64();
    delta = now - last;
    last = now;
    dx_application* application = dx_application_get();
    if (!application) {
      dx_fps_counter_on_leave_frame(g_fps_counter);
      on_shutdown_scene(ctx);
      LEAVE(DX_C_FUNCTION_NAME);
      return 1;
    }
    if (dx_application_update(application)) {
      DX_UNREFERENCE(application);
      application = NULL;
      dx_fps_counter_on_leave_frame(g_fps_counter);
      on_shutdown_scene(ctx);
      LEAVE(DX_C_FUNCTION_NAME);
      return 1;
    }
    DX_UNREFERENCE(application);
    application = NULL;
    do {
      dx_msg* msg;
      if (dx_msg_queue_pop(g_msg_queue, &msg)) {
        dx_fps_counter_on_leave_frame(g_fps_counter);
        on_shutdown_scene(ctx);
        LEAVE(DX_C_FUNCTION_NAME);
        return 1;
      }
      if (msg) {
        if (on_msg(msg)) {
          DX_UNREFERENCE(msg);
          msg = NULL;
          dx_fps_counter_on_leave_frame(g_fps_counter);
          on_shutdown_scene(ctx);
          LEAVE(DX_C_FUNCTION_NAME);
          return 1;
        }
        DX_UNREFERENCE(msg);
        msg = NULL;
      } else {
        break;
      }
    } while (true);
    dx_gl_wgl_enter_frame();
    int canvas_width, canvas_height;
    if (dx_gl_wgl_get_canvas_size(&canvas_width, &canvas_height)) {
      dx_gl_wgl_leave_frame();
      dx_fps_counter_on_leave_frame(g_fps_counter);
      on_shutdown_scene(ctx);
      LEAVE(DX_C_FUNCTION_NAME);
      return 1;
    }
    if (on_render_scene(ctx, ((dx_f32)delta)/1000.f, canvas_width, canvas_height)) {
      dx_gl_wgl_leave_frame();
      dx_fps_counter_on_leave_frame(g_fps_counter);
      on_shutdown_scene(ctx);
      LEAVE(DX_C_FUNCTION_NAME);
      return 1;
    }
    dx_gl_wgl_leave_frame();
    dx_fps_counter_on_leave_frame(g_fps_counter);
    //
    dx_string* format = dx_string_create("FPS: ${f64}\n", sizeof("FPS: ${f64}\n"));
    if (!format) {
      on_shutdown_scene(ctx);
      LEAVE(DX_C_FUNCTION_NAME);
      return 1;
    }
    dx_f64 value = dx_fps_counter_get_fps(g_fps_counter);
    dx_string* msg = dx_string_printf(format, value);
    DX_UNREFERENCE(format);
    format = NULL;
    if (!msg) {
      on_shutdown_scene(ctx);
      LEAVE(DX_C_FUNCTION_NAME);
      return 1;
    }
    dx_log(msg->bytes, msg->number_of_bytes);
    DX_UNREFERENCE(msg);
    msg = NULL;
  }
  on_shutdown_scene(ctx);
  LEAVE(DX_C_FUNCTION_NAME);
  return 0;
}

static int startup() {
  ENTER(DX_C_FUNCTION_NAME);
  if (dx_rti_initialize()) {
    return 1;
  }
  g_fps_counter = dx_fps_counter_create();
  if (!g_fps_counter) {
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return 1;
  }
  g_msg_queue = dx_msg_queue_create();
  if (!g_msg_queue) {
    DX_UNREFERENCE(g_fps_counter);
    g_fps_counter = NULL;
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return 1;
  }
  if (dx_application_startup(g_msg_queue)) {
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    DX_UNREFERENCE(g_fps_counter);
    g_fps_counter = NULL;
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return 1;
  }
  if (dx_gl_wgl_open_wm(g_msg_queue)) {
    dx_application_shutdown();
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    DX_UNREFERENCE(g_fps_counter);
    g_fps_counter = NULL;
    dx_rti_unintialize();
    LEAVE(DX_C_FUNCTION_NAME);
    return 1;
  }
  LEAVE(DX_C_FUNCTION_NAME);
  return 0;
}

static int shutdown() {
  ENTER(DX_C_FUNCTION_NAME);
  dx_gl_wgl_close_wm();
  dx_application_shutdown();
  dx_msg_queue_destroy(g_msg_queue);
  g_msg_queue = NULL;
  DX_UNREFERENCE(g_fps_counter);
  g_fps_counter = NULL;
  dx_rti_unintialize();
  LEAVE(DX_C_FUNCTION_NAME);
  return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main(int argc, char **argv) {
  if (startup()) {
    return EXIT_FAILURE;
  }
  if (run()) {
    shutdown();
    return EXIT_FAILURE;
  }
  if (shutdown()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
