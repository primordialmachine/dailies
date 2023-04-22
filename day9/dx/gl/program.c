#include "dx/gl/program.h"

// INT_MAX
#include <limits.h>

// Create a fragment program or vertex program.
static int
create_shader
  (
    GLuint* id,
    dx_gl_context* ctx,
    GLenum type,
    char const* p,
    size_t n
  );

int dx_gl_program_construct(dx_gl_context* ctx,
                            dx_gl_program* program,
                            char const* vertex_program_p,
                            size_t vertex_program_n,
                            char const* fragment_program_p,
                            size_t fragment_program_n);

void dx_gl_program_destruct(dx_gl_program* program);

static int
create_shader
  (
    GLuint* id,
    dx_gl_context* ctx,
    GLenum type,
    char const* p,
    size_t n
  )
{
  if (!id || !ctx || (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) || !p) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (n > INT_MAX) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  GLuint id1 = ctx->glCreateShader(type);
  {
    GLint m = (GLint)n;
    ctx->glShaderSource(id1, 1, &p, &m);
  }
  ctx->glCompileShader(id1);
  GLint success;
  ctx->glGetShaderiv(id1, GL_COMPILE_STATUS, &success);
  if (!success) {
    if (type == GL_VERTEX_SHADER) {
      dx_log("failed to compile vertex program\n", sizeof("failed to compile vertex program\n"));
    }
    else {
      dx_log("failed to compile fragment program\n", sizeof("failed to compile fragment program\n"));
    }
    ctx->glDeleteShader(id1);
    id1 = 0;
    return 1;
  }
  *id = id1;
  return 0;
}

int dx_gl_program_construct(dx_gl_context* ctx,
                            dx_gl_program* program,
                            char const* vertex_program_p,
                            size_t vertex_program_n,
                            char const* fragment_program_p,
                            size_t fragment_program_n)
{
  program->vertex_program_id = 0;
  program->fragment_program_id = 0;
  program->program_id = 0;
  program->ctx = ctx;

  if (create_shader(&program->vertex_program_id, ctx, GL_VERTEX_SHADER, vertex_program_p, vertex_program_n)) {
    return 1;
  }
  if (create_shader(&program->fragment_program_id, ctx, GL_FRAGMENT_SHADER, fragment_program_p, fragment_program_n)) {
    ctx->glDeleteShader(program->vertex_program_id);
    program->vertex_program_id = 0;
    return 1;
  }
  GLint success;
  {
    program->program_id = ctx->glCreateProgram();
    ctx->glAttachShader(program->program_id, program->vertex_program_id);
    ctx->glAttachShader(program->program_id, program->fragment_program_id);
    ctx->glLinkProgram(program->program_id);
    ctx->glGetProgramiv(program->program_id, GL_LINK_STATUS, &success);
    if (!success) {
      dx_log("failed to link program\n", sizeof("failed to link program program\n"));
      ctx->glDeleteShader(program->fragment_program_id);
      program->fragment_program_id = 0;
      ctx->glDeleteShader(program->vertex_program_id);
      program->vertex_program_id = 0;
      return 1;
    }
  }
  DX_OBJECT(program)->destruct = (void(*)(dx_object*)) & dx_gl_program_destruct;
  return 0;
}

void dx_gl_program_destruct(dx_gl_program* program) {
  if (program->program_id) {
    program->ctx->glDeleteProgram(program->program_id);
    program->program_id = 0;
  }

  if (program->fragment_program_id) {
    program->ctx->glDeleteShader(program->fragment_program_id);
    program->fragment_program_id = 0;
  }

  if (program->vertex_program_id) {
    program->ctx->glDeleteShader(program->vertex_program_id);
    program->vertex_program_id = 0;
  }
}

dx_gl_program* dx_gl_program_create(dx_gl_context* ctx,
                                    char const* vertex_program_p,
                                    size_t vertex_program_n,
                                    char const* fragment_program_p,
                                    size_t fragment_program_n)
{
  dx_gl_program* program = DX_GL_PROGRAM(dx_object_alloc(sizeof(dx_gl_program)));
  if (!program) {
    return NULL;
  }
  if (dx_gl_program_construct(ctx, program, vertex_program_p, vertex_program_n,
                                            fragment_program_p, fragment_program_n)) {
    DX_UNREFERENCE(program);
    program = NULL;
    return NULL;
  }
  return program;
}
