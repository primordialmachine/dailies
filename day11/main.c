// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// snprintf
#include <stdio.h>
#include "dx/core.h"
#include "dx/gl/wgl/wm.h"
#include "dx/vbinding.h"
#include "dx/buffer.h"
#include "dx/cbinding.h"
#include "dx/program.h"
#include "dx/command.h"
#include "dx/asset/mesh.h"
#include "dx/program_text.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static bool g_quit = false;

static dx_msg_queue *g_msg_queue  = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int on_msg(dx_msg* msg);

static int on_render(dx_context *context, int canvas_width, int canvas_height);

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

static dx_command_list* g_commands = NULL;

static DX_MAT4 g_projection_matrix = { 0 };
static DX_MAT4 g_view_matrix = { 0 };
static DX_MAT4 g_world_matrix = { 0 };

static dx_cbinding* create_cbinding() {
  DX_MAT4 identity;
  dx_mat4_set_identity(&identity);
  dx_cbinding* cbinding = dx_cbinding_create();
  if (!cbinding) {
    return NULL;
  }
  static const DX_VEC4 malachite = { 0.f, 255.f / 255.f, 64.f / 255.f, 1.0f }; // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
  dx_cbinding_set_vec4(cbinding, "mesh.color", &malachite);
  dx_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &identity);
  dx_cbinding_set_mat4(cbinding, "matrices.view_matrix", &identity);
  dx_cbinding_set_mat4(cbinding, "matrices.world_matrix", &identity);
  return cbinding;
}

// add commands "clear color" and "set viewport".
static int add_enter_frame_commands(dx_command_list* commands) {
  dx_command* command;
  // clear color command
  static const DX_VEC4 clear_color = { 0.f / 255.f, 191.f / 255.f, 255.f / 255.f, 0.f }; // color called "Capri" (0, 191, 255)
  command = dx_command_create_clear_color(0, 0, 640, 480, &clear_color);
  if (!command) {
    return 1;
  }
  if (dx_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return 1;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  // clear depth command
  static const float clear_depth = 1.f;
  command = dx_command_create_clear_depth(0, 0, 640, 480, 1.f);
  if (!command) {
    return 1;
  }
  if (dx_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return 1;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  // set viewport command
  command = dx_command_create_viewport(0, 0, 640, 480);
  if (!command) {
    return 1;
  }
  if (dx_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return 1;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  //
  return 0;
}

static dx_program_text* load_program(dx_string* path, dx_string* filename) {
  dx_program_text* vertex_program = NULL, * fragment_program = NULL;
  dx_program_text* program = NULL;
  {
    dx_string* format = dx_string_create("%s/%s.vs", sizeof("%s/%s.vs") - 1);
    if (!format) {
      goto on_error;
    }
    dx_string* p = dx_string_printf(format, path, filename);
    DX_UNREFERENCE(format);
    format = NULL;
    if (!p) {
      goto on_error;
    }
    vertex_program = dx_program_text_create_from_file(DX_PROGRAM_TEXT_TYPE_VERTEX, p);
    DX_UNREFERENCE(p);
    p = NULL;
    if (!vertex_program) {
      goto on_error;
    }
  }
  {
    dx_string* format = dx_string_create("%s/%s.fs", sizeof("%s/%s.fs") - 1);
    if (!format) {
      goto on_error;
    }
    dx_string* p = dx_string_printf(format, path, filename);
    DX_UNREFERENCE(format);
    format = NULL;
    if (!p) {
      goto on_error;
    }
    fragment_program = dx_program_text_create_from_file(DX_PROGRAM_TEXT_TYPE_FRAGMENT, p);
    DX_UNREFERENCE(p);
    p = NULL;
    if (!fragment_program) {
      goto on_error;
    }
  }
  {
    program = dx_program_text_create(vertex_program, fragment_program);
    DX_UNREFERENCE(vertex_program);
    vertex_program = NULL;
    DX_UNREFERENCE(fragment_program);
    fragment_program = NULL;
    if (!program) {
      goto on_error;
    }
  }
  return program;
on_error:
  if (program) {
    DX_UNREFERENCE(program);
    program = NULL;
  }
  if (fragment_program) {
    DX_UNREFERENCE(fragment_program);
    fragment_program = NULL;
  }
  if (vertex_program) {
    DX_UNREFERENCE(vertex_program);
    vertex_program = NULL;
  }
  return NULL;
}

static int add_mesh_draw_command(dx_command_list* commands,
                                 char const* name,
                                 dx_context* context) {
  dx_string* name1 = dx_string_create(name, strlen(name));
  if (!name1) {
    return 1;
  }
  dx_asset_mesh* mesh = dx_asset_mesh_create(name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!mesh) {
    return 1;
  }
  dx_buffer* buffer = dx_context_create_buffer(context);
  if (!buffer) {
    DX_UNREFERENCE(mesh);
    mesh = NULL;
    return 1;
  }
  if (dx_buffer_set_data(buffer, mesh->vertices.xyz, mesh->number_of_vertices * sizeof(DX_VEC3))) {
    DX_UNREFERENCE(buffer);
    buffer = NULL;
    DX_UNREFERENCE(mesh);
    mesh = NULL;
    return 1;
  }
  dx_vbinding* vbinding = dx_context_create_vbinding(context, buffer);
  DX_UNREFERENCE(buffer);
  buffer = NULL;
  if (!vbinding) {
    DX_UNREFERENCE(mesh);
    mesh = NULL;
    return 1;
  }
  dx_program* program = NULL;
  //
  {
    {
      dx_string* path = dx_string_create("assets/gl", sizeof("assets/gl") - 1);
      if (!path) {
        DX_UNREFERENCE(vbinding);
        vbinding = NULL;
        DX_UNREFERENCE(mesh);
        mesh = NULL;
        return 1;
      }
      dx_string* filename = dx_string_create("1", sizeof("1") - 1);
      if (!filename) {
        DX_UNREFERENCE(path);
        path = NULL;
        DX_UNREFERENCE(vbinding);
        vbinding = NULL;
        DX_UNREFERENCE(mesh);
        mesh = NULL;
        return 1;
      }
      dx_program_text* program_text = load_program(path, filename);
      DX_UNREFERENCE(filename);
      filename = NULL;
      DX_UNREFERENCE(path);
      path = NULL;
      if (!program_text) {
        DX_UNREFERENCE(vbinding);
        vbinding = NULL;
        DX_UNREFERENCE(mesh);
        mesh = NULL;
        return 1;
      }
      program = dx_context_create_program(context, program_text);
      DX_UNREFERENCE(program_text);
      program_text = NULL;
      if (!program) {
        DX_UNREFERENCE(vbinding);
        vbinding = NULL;
        DX_UNREFERENCE(mesh);
        mesh = NULL;
        return 1;
      }
    }
  }
  dx_cbinding* cbinding = create_cbinding();
  if (!cbinding) {
    DX_UNREFERENCE(program);
    program = NULL;
    DX_UNREFERENCE(vbinding);
    vbinding = NULL;
    DX_UNREFERENCE(mesh);
    mesh = NULL;
    return 1;
  }
  dx_command* command = dx_command_create_draw(vbinding, cbinding, program, 0, mesh->number_of_vertices);
  DX_UNREFERENCE(cbinding);
  cbinding = NULL;
  DX_UNREFERENCE(program);
  program = NULL;
  DX_UNREFERENCE(vbinding);
  vbinding = NULL;
  DX_UNREFERENCE(mesh);
  mesh = NULL;
  if (dx_command_list_append(commands, command)) {
    DX_UNREFERENCE(commands);
    commands = NULL;
    DX_UNREFERENCE(command);
    command = NULL;
    return 1;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  return 0;
}

static int on_startup_scene(dx_context* context) {
#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
  if (dx_math_tests()) {
    return 1;
  }
#endif
#if defined(DX_SAFE_ADD_NX_WITH_TESTS) && 1 == DX_SAFE_ADD_NX_WITH_TESTS
  if (dx_safe_add_nx_tests()) {
    return 1;
  }
#endif
#if defined(DX_SAFE_MUL_NX_WITH_TESTS) && 1 == DX_SAFE_MUL_NX_WITH_TESTS
  if (dx_safe_mul_nx_tests()) {
    return 1;
  }
#endif
  dx_mat4_set_identity(&g_projection_matrix);
  dx_mat4_set_identity(&g_view_matrix);
  dx_mat4_set_identity(&g_world_matrix);
  dx_command_list* commands = dx_command_list_create();
  if (!commands) {
    return 1;
  }
  if (add_enter_frame_commands(commands)) {
    DX_UNREFERENCE(commands);
    commands = NULL;
    return 1;
  }
  if (add_mesh_draw_command(commands, "quadriliteral", context)) {
    DX_UNREFERENCE(commands);
    commands = NULL;
    return 1;
  }
  g_commands = commands;
  return 0;
}

static int on_shutdown_scene(dx_context* context) {
  if (g_commands) {
    DX_UNREFERENCE(g_commands);
    g_commands = NULL;
  }
  return 0;
}

static int on_render_scene(dx_context *context, int canvas_width, int canvas_height) {
  {
    dx_command* command = NULL;

    // Update rectangle of the "clear color" command.
    command = dx_command_list_get_at(g_commands, 0);
    if (DX_COMMAND_KIND_CLEAR_COLOR != command->kind) {
      return 1;
    }
    command->clear_color_command.rectangle.w = (float)canvas_width;
    command->clear_color_command.rectangle.h = (float)canvas_height;

    // Update the rectangle of the "clear depth" command.
    command = dx_command_list_get_at(g_commands, 1);
    if (DX_COMMAND_KIND_CLEAR_DEPTH != command->kind) {
      return 1;
    }
    command->clear_depth_command.rectangle.w = (float)canvas_width;
    command->clear_depth_command.rectangle.h = (float)canvas_height;

    // Update rectangle of the "set viewport" command.
    command = dx_command_list_get_at(g_commands, 2);
    if (DX_COMMAND_KIND_VIEWPORT != command->kind) {
      return 1;
    }
    command->viewport_command.w = (float)canvas_width;
    command->viewport_command.h = (float)canvas_height;
  }
  {
    // Update the constants.
    dx_command* command = dx_command_list_get_at(g_commands, 3);
    if (DX_COMMAND_KIND_DRAW != command->kind) {
      return 1;
    }
    dx_cbinding* cbinding = command->draw_command.cbinding;
    dx_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &g_projection_matrix);
    dx_cbinding_set_mat4(cbinding, "matrices.view_matrix", &g_view_matrix);
    dx_cbinding_set_mat4(cbinding, "matrices.world_matrix", &g_world_matrix);
  }
  // Execute the commands.
  return dx_context_execute_commands(context, g_commands);
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
  dx_context* ctx = DX_CONTEXT(dx_gl_wgl_get_context());
  if (on_startup_scene(ctx)) {
    return 1;
  }
  while (!g_quit) {
    if (dx_gl_wgl_update_wm()) {
      dx_log("leave: run\n", sizeof("leave: run\n"));
      on_shutdown_scene(ctx);
      return 1;
    }
    dx_msg* msg;
    if (dx_msg_queue_pop(g_msg_queue, &msg)) {
      dx_log("leave: run\n", sizeof("leave: run\n"));
      on_shutdown_scene(ctx);
      return 1;
    }
    if (msg) {
      if (on_msg(msg)) {
        DX_UNREFERENCE(msg);
        msg = NULL;
        on_shutdown_scene(ctx);
        dx_log("leave: run\n", sizeof("leave: run\n"));
        return 1;
      }
      DX_UNREFERENCE(msg);
      msg = NULL;
    }
    dx_gl_wgl_enter_frame();
    int canvas_width, canvas_height;
    dx_gl_wgl_get_canvas_size(&canvas_width, &canvas_height);
    on_render_scene(ctx, canvas_width, canvas_height);
    dx_gl_wgl_leave_frame();
  }
  on_shutdown_scene(ctx);
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
