#include "dx/gl/context.h"

#include "dx/gl/buffer.h"
#include "dx/gl/binding.h"
#include "dx/gl/program.h"
#include "dx/gl/texture.h"
#include "dx/val/program_text.h"
#include "dx/val/command.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.context",
                      dx_gl_context,
                      dx_object)

static int bind_texture(dx_gl_context* ctx, dx_size unit, dx_gl_texture* texture) {
  if (texture) {
    ctx->glActiveTexture(GL_TEXTURE0 + unit);
    ctx->glBindTexture(GL_TEXTURE_2D, texture->id);
  } else {
    ctx->glActiveTexture(GL_TEXTURE0 + unit);
    ctx->glBindTexture(GL_TEXTURE_2D, 0);
  }
  return 0;
}

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

static dx_gl_texture* create_texture(dx_gl_context* ctx) {
  return dx_gl_texture_create(ctx);
}

static int execute_commands(dx_gl_context* ctx, dx_command_list* commands) {
  for (dx_size i = 0, n = dx_command_list_get_size(commands); i < n; ++i) {
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
      // assign texture to texture unit
      if (command->draw_command.material->ambient_texture) {
        dx_context_bind_texture(DX_CONTEXT(ctx), 0, command->draw_command.material->ambient_texture);
      } else {
        dx_context_bind_texture(DX_CONTEXT(ctx), 0, NULL);
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

int dx_gl_context_construct(dx_gl_context* self, void *(*link)(char const *name)) {
  dx_rti_type* _type = dx_gl_context_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_context_construct(DX_CONTEXT(self))) {
    return 1;
  }
#define DEFINE(TYPE, NAME) (self)->NAME = NULL;
#include "dx/gl/functions.i"
#undef DEFINE

#define DEFINE(TYPE, NAME) \
  (self)->NAME = (TYPE)link(#NAME); \
  if (!(self)->NAME) { \
    dx_log("unable to link " #NAME "\n", sizeof("unable to link " #NAME "\n")); \
    return 1; \
  }
#include "dx/gl/functions.i"
#undef DEFINE
  DX_CONTEXT(self)->bind_texture = (int(*)(dx_context*, dx_size, dx_val_texture*)) & bind_texture;
  DX_CONTEXT(self)->create_buffer = (dx_buffer* (*)(dx_context*)) & create_buffer;
  DX_CONTEXT(self)->create_vbinding = (dx_vbinding* (*)(dx_context*, DX_VERTEX_FORMAT, dx_buffer*)) & create_vbinding;
  DX_CONTEXT(self)->create_program = (dx_program* (*)(dx_context*, dx_program_text*)) & create_program;
  DX_CONTEXT(self)->create_texture = (dx_val_texture * (*)(dx_context*)) & create_texture;
  DX_CONTEXT(self)->execute_commands = (int (*)(dx_context*,dx_command_list*)) & execute_commands;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_gl_context_destruct(dx_gl_context* self) {
#define DEFINE(TYPE, NAME) (self)->NAME = NULL;
#include "dx/gl/functions.i"
#undef DEFINE
}
