#include "dx/gl/buffer.h"

static int dx_gl_buffer_construct(dx_gl_buffer* buffer, dx_gl_context* ctx);

static void dx_gl_buffer_destruct(dx_gl_buffer* buffer);

static int dx_gl_buffer_set_data(dx_gl_buffer* buffer, void const* p, size_t n);

static int dx_gl_buffer_construct(dx_gl_buffer* buffer, dx_gl_context* ctx) {
  if (dx_buffer_construct(DX_BUFFER(buffer), DX_CONTEXT(ctx))) {
    return 1;
  }
  ctx->glGetError(); // clear the error variable
  ctx->glGenBuffers(1, &buffer->id);
  if (GL_NO_ERROR != ctx->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 1;
  }
  DX_BUFFER(buffer)->set_data = (int(*)(dx_buffer*,void const*,size_t)) & dx_gl_buffer_set_data;
  DX_OBJECT(buffer)->destruct = (void(*)(dx_object*)) & dx_gl_buffer_destruct;
  return 0;
}

static void dx_gl_buffer_destruct(dx_gl_buffer* buffer) {
  if (buffer->id) {
    dx_gl_context* ctx = DX_GL_CONTEXT(DX_BUFFER(buffer)->context);
    ctx->glDeleteBuffers(1, &buffer->id);
    buffer->id = 0;
  }
  dx_buffer_destruct(DX_BUFFER(buffer));
}

static int dx_gl_buffer_set_data(dx_gl_buffer* buffer, void const* p, size_t n) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_BUFFER(buffer)->context);
  ctx->glGetError(); // clear the error variable
  ctx->glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
  ctx->glBufferData(GL_ARRAY_BUFFER, n, p, GL_STATIC_DRAW);
  if (GL_NO_ERROR != ctx->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 1;
  }
  return 0;
}

dx_gl_buffer* dx_gl_buffer_create(dx_gl_context* ctx) {
  dx_gl_buffer* buffer = DX_GL_BUFFER(dx_object_alloc(sizeof(dx_gl_buffer)));
  if (!buffer) {
    return NULL;
  }
  if (dx_gl_buffer_construct(buffer, ctx)) {
    DX_UNREFERENCE(buffer);
    return NULL;
  }
  return buffer;
}
