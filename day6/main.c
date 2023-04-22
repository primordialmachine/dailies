// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static bool g_quit = false;
static dx_msg_queue *g_msg_queue  = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int on_msg(dx_msg* msg);

static int run();

static int startup();

static int shutdown();

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

static int run() {
  dx_log("enter: run\n", sizeof("enter: run\n"));
  {
    dx_msg* msg;
    
    // create the "emit" message.
    msg = DX_MSG(dx_emit_msg_create("Hello, World!\n", sizeof("Hello, World!\n")));
    if (!msg) {
      dx_log("leave: run\n", sizeof("leave: run\n"));
      return 1;
    }
    if (dx_msg_queue_push(g_msg_queue, msg)) {
      DX_UNREFERENCE(msg);
      msg = NULL;
      dx_log("leave: run\n", sizeof("leave: run\n"));
      return 1;
    }
    DX_UNREFERENCE(msg);
    msg = NULL;
    
    // create the "quit" message.
    msg = DX_MSG(dx_quit_msg_create());
    if (!msg) {
      dx_log("leave: run\n", sizeof("leave: run\n"));
      return 1;
    }
    if (dx_msg_queue_push(g_msg_queue, msg)) {
      DX_UNREFERENCE(msg);
      msg = NULL;
      dx_log("leave: run\n", sizeof("leave: run\n"));
      return 1;
    }
    DX_UNREFERENCE(msg);
    msg = NULL;
  }
  while (!g_quit) {
    dx_msg* msg;
    if (dx_msg_queue_pop(g_msg_queue, &msg)) {
      dx_log("leave: run\n", sizeof("leave: run\n"));
      return 1;
    }
    if (msg) {
      if (on_msg(msg)) {
        DX_UNREFERENCE(msg);
        msg = NULL;
        dx_log("leave: run\n", sizeof("leave: run\n"));
        return 1;
      }
      DX_UNREFERENCE(msg);
      msg = NULL;
    }
  }
  dx_log("leave: run\n", sizeof("leave: run\n"));
  return 0;
}

static int startup() {
  dx_log("enter: startup\n", sizeof("enter: startup\n"));
  g_msg_queue = dx_msg_queue_create();
  if (!g_msg_queue) {
    dx_log("leave: startup\n", sizeof("leave: startup\n"));
    return 1;
  }
  dx_log("leave: startup\n", sizeof("leave: startup\n"));
  return 0;
}

static int shutdown() {
  dx_log("enter: shutdown\n", sizeof("enter: shutdown\n"));
  dx_msg_queue_destroy(g_msg_queue);
  g_msg_queue = NULL;
  dx_log("leave: shutdown\n", sizeof("leave: shutdown\n"));
  return 0;
}

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
