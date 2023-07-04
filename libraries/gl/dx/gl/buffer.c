#include "dx/gl/buffer.h"

static int dx_gl_buffer_construct(dx_gl_buffer* buffer, dx_gl_context* context);

static int dx_gl_buffer_set_data(dx_gl_buffer* buffer, void const* p, dx_size n);

static int dx_gl_buffer_construct(dx_gl_buffer* buffer, dx_gl_context* context) {
  if (dx_buffer_construct(DX_BUFFER(buffer), DX_CONTEXT(context))) {
    return 1;
  }
  context->glGetError(); // clear the error variable
  context->glGenBuffers(1, &buffer->id);
  if (GL_NO_ERROR != context->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 1;
  }
  DX_BUFFER(buffer)->set_data = (int(*)(dx_buffer*,void const*, dx_size)) & dx_gl_buffer_set_data;
  return 0;
}

static void dx_gl_buffer_destruct(dx_gl_buffer* buffer) {
  if (buffer->id) {
    dx_gl_context* context = DX_GL_CONTEXT(DX_BUFFER(buffer)->context);
    context->glDeleteBuffers(1, &buffer->id);
    buffer->id = 0;
  }
}

static int dx_gl_buffer_set_data(dx_gl_buffer* buffer, void const* p, dx_size n) {
  dx_gl_context* context = DX_GL_CONTEXT(DX_BUFFER(buffer)->context);
  context->glGetError(); // clear the error variable
  context->glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
  context->glBufferData(GL_ARRAY_BUFFER, n, p, GL_STATIC_DRAW);
  if (GL_NO_ERROR != context->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 1;
  }
  return 0;
}

dx_gl_buffer* dx_gl_buffer_create(dx_gl_context* context) {
  dx_gl_buffer* buffer = DX_GL_BUFFER(dx_object_alloc(sizeof(dx_gl_buffer)));
  if (!buffer) {
    return NULL;
  }
  if (dx_gl_buffer_construct(buffer, context)) {
    DX_UNREFERENCE(buffer);
    return NULL;
  }
  return buffer;
}
