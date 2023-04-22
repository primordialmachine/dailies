#include "dx/gl/binding.h"

#include "dx/gl/buffer.h"

static int dx_gl_binding_activate(dx_gl_binding* binding);

static void dx_gl_binding_destruct(dx_gl_binding* binding);

static int dx_gl_binding_construct(dx_gl_binding* binding, DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer);

static int dx_gl_binding_activate(dx_gl_binding* binding) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_VBINDING(binding)->context);
  ctx->glBindVertexArray(binding->id);
  return 0;
}

static int dx_gl_binding_construct(dx_gl_binding* binding, DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer) {
  if (dx_vbinding_construct(DX_VBINDING(binding), DX_BUFFER(buffer))) {
    return 1;
  }

  binding->vertex_format = vertex_format;

  dx_gl_context* ctx = DX_GL_CONTEXT(DX_VBINDING(binding)->context);

  ctx->glGetError();
  ctx->glGenVertexArrays(1, &binding->id);
  if (GL_NO_ERROR != ctx->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 1;
  }
  ctx->glBindVertexArray(binding->id);
  // the vertex attribute 0 is activated and reads from the specified buffer data with the the specified format
  // https://www.khronos.org/opengl/wiki/Vertex_Specification
  ctx->glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
  switch (binding->vertex_format) {
  case DX_VERTEX_FORMAT_POSITION: {
    size_t stride = 3 * sizeof(float);
    size_t offset = 0;
    
    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(float);
  } break;
  case DX_VERTEX_FORMAT_POSITION_COLOR: {
    size_t stride = 3 * sizeof(float) + 4 * sizeof(float);
    size_t offset = 0;
    
    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(float);
    
    ctx->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(1);
    offset += 4 * sizeof(float);
  } break;
  case DX_VERTEX_FORMAT_POSITION_TEXTURE: {
    size_t stride = 3 * sizeof(float) + 2 * sizeof(float);
    size_t offset = 0;

    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(float);

    ctx->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(1);
    offset += 2 * sizeof(float);
  } break;
  case DX_VERTEX_FORMAT_COLOR:
  default: {
    dx_set_error(DX_INVALID_ARGUMENT);
    ctx->glDeleteVertexArrays(1, &binding->id);
    binding->id = 0;
    return 1;
  } break;
  };
  if (GL_NO_ERROR != ctx->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    ctx->glDeleteVertexArrays(1, &binding->id);
    binding->id = 0;
    return 1;
  }

  DX_VBINDING(binding)->activate = (int(*)(dx_vbinding*)) & dx_gl_binding_activate;
  DX_OBJECT(binding)->destruct = (void(*)(dx_object*)) & dx_gl_binding_destruct;

  return 0;
}

static void dx_gl_binding_destruct(dx_gl_binding* binding) {
  dx_gl_context* context = DX_GL_CONTEXT(DX_VBINDING(binding)->context);
  if (binding->id) {
    context->glDeleteVertexArrays(1, &binding->id);
    binding->id = 0;
  }
  dx_vbinding_destruct(DX_VBINDING(binding));
}

dx_gl_binding* dx_gl_binding_create(DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer) {
  dx_gl_binding* binding = DX_GL_BINDING(dx_object_alloc(sizeof(dx_gl_binding)));
  if (!binding) {
    return NULL;
  }
  if (dx_gl_binding_construct(binding, vertex_format, buffer)) {
    DX_UNREFERENCE(binding);
    binding = NULL;
    return NULL;
  }
  return binding;
}
