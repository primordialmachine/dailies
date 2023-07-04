#include "dx/gl/binding.h"

#include "dx/gl/buffer.h"

static int dx_gl_binding_activate(dx_gl_binding* binding);

static int dx_gl_binding_construct(dx_gl_binding* binding, DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer);

static int dx_gl_binding_activate(dx_gl_binding* binding) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_VBINDING(binding)->context);
  ctx->glBindVertexArray(binding->id);
  return 0;
}

static int dx_gl_binding_construct(dx_gl_binding* self, DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer) {
  if (dx_vbinding_construct(DX_VBINDING(self), DX_BUFFER(buffer))) {
    return 1;
  }

  self->vertex_format = vertex_format;

  dx_gl_context* ctx = DX_GL_CONTEXT(DX_VBINDING(self)->context);

  ctx->glGetError();
  ctx->glGenVertexArrays(1, &self->id);
  if (GL_NO_ERROR != ctx->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 1;
  }
  ctx->glBindVertexArray(self->id);
  // the vertex attribute 0 is activated and reads from the specified buffer data with the the specified format
  // https://www.khronos.org/opengl/wiki/Vertex_Specification
  ctx->glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
  switch (self->vertex_format) {
  case DX_VERTEX_FORMAT_POSITION_XYZ: {
    dx_size stride = 3 * sizeof(dx_f32);
    dx_size offset = 0;
    
    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(dx_f32);
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA: {
    dx_size stride = 3 * sizeof(dx_f32) + 4 * sizeof(dx_f32);
    dx_size offset = 0;
    
    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(dx_f32);
    
    ctx->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(1);
    offset += 4 * sizeof(dx_f32);
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_UV: {
    dx_size stride = 3 * sizeof(dx_f32) + 2 * sizeof(dx_f32);
    dx_size offset = 0;

    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(dx_f32);

    ctx->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(1);
    offset += 2 * sizeof(dx_f32);
  } break;
  case DX_VERTEX_FORMAT_AMBIENT_RGBA:
  default: {
    dx_set_error(DX_INVALID_ARGUMENT);
    ctx->glDeleteVertexArrays(1, &self->id);
    self->id = 0;
    return 1;
  } break;
  };
  if (GL_NO_ERROR != ctx->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    ctx->glDeleteVertexArrays(1, &self->id);
    self->id = 0;
    return 1;
  }

  DX_VBINDING(self)->activate = (int(*)(dx_vbinding*)) & dx_gl_binding_activate;
  return 0;
}

static void dx_gl_binding_destruct(dx_gl_binding* self) {
  dx_gl_context* context = DX_GL_CONTEXT(DX_VBINDING(self)->context);
  if (self->id) {
    context->glDeleteVertexArrays(1, &self->id);
    self->id = 0;
  }
}

dx_gl_binding* dx_gl_binding_create(DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer) {
  dx_gl_binding* self = DX_GL_BINDING(dx_object_alloc(sizeof(dx_gl_binding)));
  if (!self) {
    return NULL;
  }
  if (dx_gl_binding_construct(self, vertex_format, buffer)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
