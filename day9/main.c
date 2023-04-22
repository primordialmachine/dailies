// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
#include "dx/core.h"
#include "dx/gl/wgl/wm.h"
#include "dx/gl/program.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static bool g_quit = false;

static dx_msg_queue *g_msg_queue  = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int on_msg(dx_msg* msg);

static int on_render(dx_context* context, int canvas_width, int canvas_height);

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

static const float vertices[] = {
  -0.5f, -0.5f, 0.0f,
  +0.5f, -0.5f, 0.0f,
  +0.0f, +0.5f, 0.0f
};

static const char* vertex_program_source =
  "#version 330 core\n"
  "layout (location = 0) in vec3 position;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
  "}\0";

static const char* fragment_program_source =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(0.f, 255.f/255.f, 64.f/255.f, 1.0f);\n" // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
  "}\0";

static int on_render(dx_context *context, int canvas_width, int canvas_height) {
  dx_context_set_viewport(context, 0.f, 0.f, (float)canvas_width, (float)canvas_height);
  dx_context_clear_color(context, 0.f / 255.f, 191.f / 255.f, 255.f / 255.f, 0.f); // color called "Capri" (0, 191, 255)


  dx_gl_program* gl_program = dx_gl_program_create(DX_GL_CONTEXT(context),
                                                   vertex_program_source, strlen(vertex_program_source),
                                                   fragment_program_source, strlen(fragment_program_source));
  if (!gl_program) {
    return 1;
  }
  GLuint vbo_id = 0;
  {
    while (DX_GL_CONTEXT(context)->glGetError())
    {/*Intentionally empty.*/}
    DX_GL_CONTEXT(context)->glGenBuffers(1, &vbo_id);
    if (GL_NO_ERROR != DX_GL_CONTEXT(context)->glGetError()) {
      DX_UNREFERENCE(gl_program);
      gl_program = NULL;
      return 1;
    }
    DX_GL_CONTEXT(context)->glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    // set the buffer data
    DX_GL_CONTEXT(context)->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  }

  GLint vao_id;
  DX_GL_CONTEXT(context)->glGenVertexArrays(1, &vao_id);
  DX_GL_CONTEXT(context)->glBindVertexArray(vao_id);
  // 1. bind the buffer
  DX_GL_CONTEXT(context)->glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
  // 2. set the vertex attribute pointers
  // https://www.khronos.org/opengl/wiki/Vertex_Specification
  DX_GL_CONTEXT(context)->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  DX_GL_CONTEXT(context)->glEnableVertexAttribArray(0);

  // activate binding and activate program then render
  DX_GL_CONTEXT(context)->glUseProgram(gl_program->program_id);
  DX_GL_CONTEXT(context)->glBindVertexArray(vao_id);
  DX_GL_CONTEXT(context)->glDrawArrays(GL_TRIANGLES, 0, 3);


  DX_UNREFERENCE(gl_program);
  gl_program = NULL;
  if (vao_id) {
    DX_GL_CONTEXT(context)->glDeleteVertexArrays(1, &vao_id);
    vao_id = 0;
  }
  if (vbo_id) {
    DX_GL_CONTEXT(context)->glDeleteBuffers(1, &vbo_id);
    vbo_id = 0;
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
  }
  while (!g_quit) {
    if (dx_gl_wgl_update_wm()) {
      dx_log("leave: run\n", sizeof("leave: run\n"));
      return 1;
    }
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
    dx_gl_wgl_enter_frame();
    int canvas_width, canvas_height;
    dx_gl_wgl_get_canvas_size(&canvas_width, &canvas_height);
    dx_context* ctx = DX_CONTEXT(dx_gl_wgl_get_context());
    on_render(ctx, canvas_width, canvas_height);
    dx_gl_wgl_leave_frame();
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
  if (dx_application_startup(g_msg_queue)) {
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
    dx_log("leave: startup\n", sizeof("leave: startup\n"));
    return 1;
  }
  if (dx_gl_wgl_open_wm(g_msg_queue)) {
    dx_application_shutdown();
    dx_msg_queue_destroy(g_msg_queue);
    g_msg_queue = NULL;
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
