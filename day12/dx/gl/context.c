#include "dx/gl/context.h"

#include "dx/gl/buffer.h"
#include "dx/gl/binding.h"
#include "dx/gl/program.h"
#include "dx/val/program_text.h"
#include "dx/val/command.h"

static dx_gl_buffer* create_buffer(dx_gl_context* ctx) {
  return dx_gl_buffer_create(ctx);
}

static dx_gl_binding* create_vbinding(dx_gl_context* ctx, DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer) {
  return dx_gl_binding_create(vertex_format, buffer);
}

static dx_gl_program* create_program(dx_gl_context* ctx, dx_program_text* program_text) {
  if (!program_text || DX_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT != program_text->type) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  return dx_gl_program_create(ctx, program_text->vertex_program_text->program_text, program_text->fragment_program_text->program_text);
}

static int execute_commands(dx_gl_context* ctx, dx_command_list* commands) {
  for (size_t i = 0, n = dx_command_list_get_size(commands); i < n; ++i) {
    dx_command* command = dx_command_list_get_at(commands, i);
    switch (command->kind) {
    case DX_COMMAND_KIND_CLEAR_COLOR: {
      ctx->glEnable(GL_SCISSOR_TEST);
      ctx->glScissor(command->clear_color_command.rectangle.l,
                     command->clear_color_command.rectangle.b,
                     command->clear_color_command.rectangle.w,
                     command->clear_color_command.rectangle.h);
      ctx->glClearColor(command->clear_color_command.color.e[0],
                        command->clear_color_command.color.e[1],
                        command->clear_color_command.color.e[2],
                        command->clear_color_command.color.e[3]);
      ctx->glClear(GL_COLOR_BUFFER_BIT);
    } break;
    case DX_COMMAND_KIND_CLEAR_DEPTH: {
      ctx->glEnable(GL_SCISSOR_TEST);
      ctx->glScissor(command->clear_depth_command.rectangle.l,
                     command->clear_depth_command.rectangle.b,
                     command->clear_depth_command.rectangle.w,
                     command->clear_depth_command.rectangle.h);
      ctx->glClearDepthf(1.f);
      ctx->glClear(GL_DEPTH_BUFFER_BIT);
    } break;
    case DX_COMMAND_KIND_DRAW: {
      // Enable culling polygons that are back-facing.
      ctx->glEnable(GL_CULL_FACE);
      ctx->glCullFace(GL_BACK);
      // Enable depth testing.
      // An incoming fragment passes the depth test if its depth value is smaller than the corresponding value in the depth buffer.
      // The corresponding value in the depth buffer is then replaced by the incoming fragment's depth value.
      ctx->glEnable(GL_DEPTH_TEST);
      ctx->glDepthFunc(GL_LESS);
      if (ctx->glGetError()) {
        int x = 0;
      }
      // apply the constant binding to the program.
      if (dx_program_bind(command->draw_command.program, command->draw_command.cbinding)) {
        return 1;
      }
      // activate binding and activate program, then render.
      if (dx_program_activate(command->draw_command.program)) {
        return 1;
      }
      if (dx_vbinding_activate(command->draw_command.vbinding)) {
        return 1;
      }
      ctx->glDrawArrays(GL_TRIANGLES, command->draw_command.start, command->draw_command.length);
    } break;
    case DX_COMMAND_KIND_VIEWPORT: {
      ctx->glViewport(command->viewport_command.l,
                      command->viewport_command.b,
                      command->viewport_command.w,
                      command->viewport_command.h);
    } break;
    case DX_COMMAND_KIND_IDLE: {
      // Do nothing.
    } break;
    default: {
      return 1;
    } break;
    }
  }
  return 0;
}

int dx_gl_context_construct(dx_gl_context* ctx, void *(*link)(char const *name)) {
  if (dx_context_construct(DX_CONTEXT(ctx))) {
    return 1;
  }
#define DEFINE(TYPE, NAME) (ctx)->NAME = NULL;
#include "dx/gl/functions.i"
#undef DEFINE

#define DEFINE(TYPE, NAME) \
  (ctx)->NAME = (TYPE)link(#NAME); \
  if (!(ctx)->NAME) { \
    dx_log("unable to link " #NAME "\n", sizeof("unable to link " #NAME "\n")); \
    return 1; \
  }
#include "dx/gl/functions.i"
#undef DEFINE
  DX_CONTEXT(ctx)->create_buffer = (dx_buffer* (*)(dx_context*)) & create_buffer;
  DX_CONTEXT(ctx)->create_vbinding = (dx_vbinding* (*)(dx_context*, DX_VERTEX_FORMAT, dx_buffer*)) & create_vbinding;
  DX_CONTEXT(ctx)->create_program = (dx_program* (*)(dx_context*, dx_program_text*)) & create_program;
  DX_CONTEXT(ctx)->execute_commands = (int (*)(dx_context*,dx_command_list*)) & execute_commands;
  DX_OBJECT(ctx)->destruct = (void(*)(dx_object*)) & dx_gl_context_destruct;
  return 0;
}

void dx_gl_context_destruct(dx_gl_context* ctx) {
#define DEFINE(TYPE, NAME) (ctx)->NAME = NULL;
#include "dx/gl/functions.i"
#undef DEFINE
  dx_context_destruct(DX_CONTEXT(ctx));
}
