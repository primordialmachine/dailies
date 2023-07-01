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

#include "dx/application.h"
#include "dx/gl/wgl/wm.h"
#include "dx/scenes/mesh_viewer_scene.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static bool g_quit = false;

static dx_msg_queue *g_msg_queue  = NULL;

static size_t g_scene_index = 0;

static dx_scene* g_scenes[2] = { NULL, NULL };

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int on_msg(dx_msg* msg);

static int on_render_scene(dx_context* context, dx_f32 delta_seconds, int canvas_width, int canvas_height);

static int run();

static int startup();

static int shutdown();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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

#if defined(_WIN32)
  #include "dx/gl/wgl/wm.h"
#else
  #error("environment not (yet) supported")
#endif

int dx_application_startup(dx_msg_queue* msg_queue) {
  return dx_gl_wgl_application_startup(msg_queue);
}

void dx_application_shutdown() {
  dx_gl_wgl_application_shutdown();
}

dx_application* dx_application_get() {
  return (dx_application*)dx_gl_wgl_application_get();
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int on_msg(dx_msg* msg) {
  switch (dx_msg_get_flags(msg)) {
  case DX_MSG_TYPE_EMIT: {
    dx_emit_msg* emit_msg = DX_EMIT_MSG(msg);
    char const* p; size_t n;
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
        g_scene_index = (g_scene_index + 1) % 2;
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
  g_scenes[0] = DX_SCENE(dx_mesh_viewer_scene_create("./assets/cube.adl"));
  if (!g_scenes[0]) {
    return 1;
  }
  g_scenes[1] = DX_SCENE(dx_mesh_viewer_scene_create("./assets/octahedron.adl"));
  if (!g_scenes[1]) {
    DX_UNREFERENCE(g_scenes[0]);
    g_scenes[0] = NULL;
    return 1;
  }
  //
  if (dx_scene_startup(g_scenes[0], context)) {
    DX_UNREFERENCE(g_scenes[1]);
    g_scenes[1] = NULL;
    DX_UNREFERENCE(g_scenes[0]);
    g_scenes[0] = NULL;
    return 1;
  }
  if (dx_scene_startup(g_scenes[1], context)) {
    dx_scene_shutdown(g_scenes[0], context);
    DX_UNREFERENCE(g_scenes[1]);
    g_scenes[1] = NULL;
    DX_UNREFERENCE(g_scenes[0]);
    g_scenes[0] = NULL;
    return 1;
  }
  return 0;
}

static int on_shutdown_scene(dx_context* context) {
  if (g_scenes[1]) {
    dx_scene_shutdown(g_scenes[1], context);
  }
  if (g_scenes[0]) {
    dx_scene_shutdown(g_scenes[0], context);
  }
  //
  if (g_scenes[1]) {
    DX_UNREFERENCE(g_scenes[1]);
    g_scenes[1] = NULL;
  }
  if (g_scenes[0]) {
    DX_UNREFERENCE(g_scenes[0]);
    g_scenes[0] = NULL;
  }
  return 0;
}

static int on_render_scene(dx_context* context, dx_f32 delta_seconds, int canvas_width, int canvas_height) {
  if (dx_scene_render(g_scenes[g_scene_index], context, delta_seconds, canvas_width, canvas_height)) {
    return 1;
  }
  return 0;
}

static int run() {
  dx_log("enter: run\n", sizeof("enter: run\n"));
  {
    dx_msg* msg;
    
    // create the "emit" message.
    msg = DX_MSG(dx_emit_msg_create("Hello, World!\n", sizeof("Hello, World!\n")));
    if (!msg) {
      dx_log("leave: run (failure)\n", sizeof("leave: run (failure)\n"));
      return 1;
    }
    if (dx_msg_queue_push(g_msg_queue, msg)) {
      DX_UNREFERENCE(msg);
      msg = NULL;
      dx_log("leave: run (failure)\n", sizeof("leave: run (failure)\n"));
      return 1;
    }
    DX_UNREFERENCE(msg);
    msg = NULL;
  }
  dx_context* ctx = DX_CONTEXT(dx_gl_wgl_get_context());
  if (on_startup_scene(ctx)) {
    dx_log("leave: run (failure)\n", sizeof("leave: run (failure)\n"));
    return 1;
  }
  uint64_t last = GetTickCount64();
  uint64_t now = last;
  uint64_t delta = now - last;
  while (!g_quit) {
    now = GetTickCount64();
    delta = now - last;
    last = now;

    if (dx_gl_wgl_update_wm()) {
      on_shutdown_scene(ctx);
      dx_log("leave: run (failure)\n", sizeof("leave: run (failure)\n"));
      return 1;
    }
    do {
      dx_msg* msg;
      if (dx_msg_queue_pop(g_msg_queue, &msg)) {
        on_shutdown_scene(ctx);
        dx_log("leave: run (failure)\n", sizeof("leave: run (failure)\n"));
        return 1;
      }
      if (msg) {
        if (on_msg(msg)) {
          DX_UNREFERENCE(msg);
          msg = NULL;
          on_shutdown_scene(ctx);
          dx_log("leave: run (failure)\n", sizeof("leave: run (failure)\n"));
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
      on_shutdown_scene(ctx);
      dx_log("leave: run (failure)\n", sizeof("leave: run (failure)\n"));
      return 1;
    }
    if (on_render_scene(ctx, ((dx_f32)delta)/1000.f, canvas_width, canvas_height)) {
      dx_gl_wgl_leave_frame();
      on_shutdown_scene(ctx);
      dx_log("leave: run (failure)\n", sizeof("leave: run (failure)\n"));
      return 1;
    }
    dx_gl_wgl_leave_frame();
  }
  on_shutdown_scene(ctx);
  dx_log("leave: run (success)\n", sizeof("leave: run (success)\n"));
  return 0;
}

static int startup() {
  dx_log("enter: startup\n", sizeof("enter: startup\n"));
  if (dx_rti_initialize()) {
    return 1;
  }
  g_msg_queue = dx_msg_queue_create();
  if (!g_msg_queue) {
    dx_rti_unintialize();
    dx_log("leave: startup\n", sizeof("leave: startup\n"));
    return 1;
  }
  if (dx_application_startup(g_msg_queue)) {
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    dx_rti_unintialize();
    dx_log("leave: startup\n", sizeof("leave: startup\n"));
    return 1;
  }
  if (dx_gl_wgl_open_wm(g_msg_queue)) {
    dx_application_shutdown();
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    dx_rti_unintialize();
    dx_log("leave: startup\n", sizeof("leave: startup\n"));
    return 1;
  }
  dx_log("leave: startup\n", sizeof("leave: startup\n"));
  return 0;
}

static int shutdown() {
  dx_log("enter: shutdown\n", sizeof("enter: shutdown\n"));
  dx_gl_wgl_close_wm();
  dx_application_shutdown();
  dx_msg_queue_destroy(g_msg_queue);
  g_msg_queue = NULL;
  dx_rti_unintialize();
  dx_log("leave: shutdown\n", sizeof("leave: shutdown\n"));
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
