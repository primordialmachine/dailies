#include "dx/gl/program.h"

// INT_MAX
#include <limits.h>

#include "dx/val/cbinding.h"
#include "dx/gl/context.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.program",
                      dx_gl_program,
                      dx_program)

/// @brief Defined and equal to @a 1, then do not display shader logs to the user.
#define DX_GL_PROGRAM_WITH_LOG_EMISSION_DISABLED (0)

// Create a fragment program or vertex program.
static int
create_shader
  (
    GLuint* id,
    dx_gl_context* ctx,
    GLenum type,
    dx_string* program_text
  );

static int
emit_log
  (
    dx_gl_context* ctx,
    GLuint id
  );

static int dx_gl_program_bind(dx_gl_program* program, dx_cbinding* cbinding);

static int dx_gl_program_activate(dx_gl_program* program);

static int dx_gl_program_construct(dx_gl_program* program,
                                   dx_gl_context* ctx,
                                   dx_string* vertex_program_text,
                                   dx_string* fragment_program_text);

static int
create_shader
  (
    GLuint* id,
    dx_gl_context* ctx,
    GLenum type,
    dx_string *program_text
  )
{
  if (!id || !ctx || (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) || !program_text) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (program_text->number_of_bytes > INT_MAX) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  GLuint id1 = ctx->glCreateShader(type);
  {
    dx_byte_array byte_array;
    if (dx_byte_array_initialize(&byte_array)) {
      ctx->glDeleteShader(id1);
      id1 = 0;
      return 1;
    }
    if (dx_byte_array_append(&byte_array, "#version 330 core\n", sizeof("#version 330 core\n") - 1)) {
      dx_byte_array_uninitialize(&byte_array);
      ctx->glDeleteShader(id1);
      id1 = 0;
      return 1;
    }
    if (dx_byte_array_append(&byte_array, program_text->bytes, program_text->number_of_bytes)) {
      dx_byte_array_uninitialize(&byte_array);
      ctx->glDeleteShader(id1);
      id1 = 0;
      return 1;
    }
    GLint const m[] = { (GLint)byte_array.size };
    GLchar const *q[] = { byte_array.elements };
    ctx->glShaderSource(id1, 1, q, m);
    dx_byte_array_uninitialize(&byte_array);
  }
  ctx->glCompileShader(id1);
  GLint success;
  ctx->glGetShaderiv(id1, GL_COMPILE_STATUS, &success);
  if (!success) {
    if (type == GL_VERTEX_SHADER) {
      dx_log("failed to compile vertex program\n", sizeof("failed to compile vertex program\n"));
    } else {
      dx_log("failed to compile fragment program\n", sizeof("failed to compile fragment program\n"));
    }
    emit_log(ctx, id1);
    ctx->glDeleteShader(id1);
    id1 = 0;
    return 1;
  }
  *id = id1;
  return 0;
}

static int
emit_log
  (
    dx_gl_context* ctx,
    GLuint id
  )
{
#if defined(DX_GL_PROGRAM_WITH_LOG_EMISSION_DISABLED) && 1 == DX_GL_PROGRAM_WITH_LOG_EMISSION_DISABLED
  // Do not display shader logs to the user.
#else
  GLint l; // including the zero terminator
  ctx->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l);
  char* p = dx_memory_allocate(sizeof(char) * l);
  if (!p) {
    return 1;
  }
  ctx->glGetShaderInfoLog(id, l, NULL, p);
  dx_log(p, strlen(p));
  dx_memory_deallocate(p);
#endif
  return 0;
}

static int dx_gl_program_bind(dx_gl_program* program, dx_cbinding* cbinding) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_PROGRAM(program)->ctx);

  ctx->glUseProgram(program->program_id);

  dx_cbinding_iter it = dx_cbinding_get_iter(cbinding);
  if (dx_get_error()) {
    return 1;
  }
  while (dx_cbinding_iter_has_value(&it)) {
    char const* name = dx_cbinding_iter_get_name(&it);
    if (!name) {
      return 1;
    }
    GLint location = ctx->glGetUniformLocation(program->program_id, name);
    if (-1 == location) {
      dx_cbinding_iter_next(&it);
      continue;
    }
    uint8_t tag = dx_cbinding_iter_get_tag(&it);
    switch (tag) {
    case DX_CBINDING_TYPE_VEC3: {
      DX_VEC3 v;
      if (dx_cbinding_iter_get_vec3(&it, &v)) {
        return 1;
      }
      ctx->glUniform3fv(location, 1, &(v.e[0]));
    } break;
    case DX_CBINDING_TYPE_VEC4: {
      DX_VEC4 v;
      if (dx_cbinding_iter_get_vec4(&it, &v)) {
        return 1;
      }
      ctx->glUniform4fv(location, 1, &(v.e[0]));
    } break;
    case DX_CBINDING_TYPE_MAT4: {
      DX_MAT4 a;
      if (dx_cbinding_iter_get_mat4(&it, &a)) {
        return 1;
      }
      ctx->glUniformMatrix4fv(location, 1, GL_TRUE, &(a.e[0][0]));
    } break;
    case DX_CBINDING_TYPE_TEXTURE_INDEX: {
      dx_size i;
      if (dx_cbinding_iter_get_texture_index(&it, &i)) {
        return 1;
      }
      GLint max_texture_units;
      ctx->glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
      if (i >= max_texture_units) {
        return 1;
      }
      ctx->glUniform1i(location, (GLint)i);
    } break;
    case DX_CBINDING_TYPE_EMPTY: {
      if (dx_get_error()) {
        return 1;
      }
    } break;
    default: {
      dx_set_error(DX_INVALID_ARGUMENT);
      return 1;
    } break;
    };
    dx_cbinding_iter_next(&it);
  }
  if (dx_get_error()) {
    return 1;
  }
  return 0;
}

static int dx_gl_program_activate(dx_gl_program* self) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_PROGRAM(self)->ctx);
  ctx->glUseProgram(self->program_id);
  return 0;
}

static int dx_gl_program_construct(dx_gl_program* self,
                                   dx_gl_context* ctx,
                                   dx_string* vertex_program_text,
                                   dx_string* fragment_program_text)
{
  dx_rti_type* _type = dx_gl_program_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_program_construct(DX_PROGRAM(self), DX_CONTEXT(ctx))) {
    return 1;
  }
  self->vertex_program_id = 0;
  self->fragment_program_id = 0;
  self->program_id = 0;

  if (create_shader(&self->vertex_program_id, ctx, GL_VERTEX_SHADER, vertex_program_text)) {
    return 1;
  }
  if (create_shader(&self->fragment_program_id, ctx, GL_FRAGMENT_SHADER, fragment_program_text)) {
    ctx->glDeleteShader(self->vertex_program_id);
    self->vertex_program_id = 0;
    return 1;
  }
  GLint success;
  {
    self->program_id = ctx->glCreateProgram();
    ctx->glAttachShader(self->program_id, self->vertex_program_id);
    ctx->glAttachShader(self->program_id, self->fragment_program_id);
    ctx->glLinkProgram(self->program_id);
    ctx->glGetProgramiv(self->program_id, GL_LINK_STATUS, &success);
    if (!success) {
      dx_log("failed to link program\n", sizeof("failed to link program program\n"));
      ctx->glDeleteShader(self->fragment_program_id);
      self->fragment_program_id = 0;
      ctx->glDeleteShader(self->vertex_program_id);
      self->vertex_program_id = 0;
      return 1;
    }
  }
  DX_PROGRAM(self)->bind = (int(*)(dx_program*, dx_cbinding*)) & dx_gl_program_bind;
  DX_PROGRAM(self)->activate = (int(*)(dx_program*)) & dx_gl_program_activate;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_gl_program_destruct(dx_gl_program* self) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_PROGRAM(self)->ctx);
  if (self->program_id) {
    ctx->glDeleteProgram(self->program_id);
    self->program_id = 0;
  }

  if (self->fragment_program_id) {
    ctx->glDeleteShader(self->fragment_program_id);
    self->fragment_program_id = 0;
  }

  if (self->vertex_program_id) {
    ctx->glDeleteShader(self->vertex_program_id);
    self->vertex_program_id = 0;
  }
}

dx_gl_program* dx_gl_program_create(dx_gl_context* ctx,
                                    dx_string* vertex_program_text,
                                    dx_string* fragment_program_text)
{
  dx_gl_program* self = DX_GL_PROGRAM(dx_object_alloc(sizeof(dx_gl_program)));
  if (!self) {
    return NULL;
  }
  if (dx_gl_program_construct(self, ctx, vertex_program_text, fragment_program_text)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
