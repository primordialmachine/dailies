#if !defined(DX_CONTEXT_H_INCLUDED)
#define DX_CONTEXT_H_INCLUDED

#include "dx/core.h"
typedef struct dx_buffer dx_buffer;
typedef struct dx_vbinding dx_vbinding;
typedef struct dx_command dx_command;
typedef struct dx_command_list dx_command_list;
typedef struct dx_program dx_program;
typedef struct dx_program_text dx_program_text;

typedef struct dx_context dx_context;
static inline dx_context* DX_CONTEXT(void* p) {
  return (dx_context*)p;
}

struct dx_context {
  dx_object _parent;
  dx_buffer* (*create_buffer)(dx_context*);
  dx_vbinding* (*create_vbinding)(dx_context*, DX_VERTEX_FORMAT vertex_format, dx_buffer* buffer);
  dx_program* (*create_program)(dx_context*, dx_program_text*);
  int (*execute_commands)(dx_context*, dx_command_list*);
};

int dx_context_construct(dx_context* context);

void dx_context_destruct(dx_context* context);

/// Create a buffer.
/// @param context A pointer to this context.
/// @return The buffer.
/// @return A pointer to the buffer on success. A null pointero n failure.
dx_buffer* dx_context_create_buffer(dx_context* context);

/// Create a variable binding.
/// @param context A pointer to this context.
/// @param vertex_format The vertex format of the buffer.
/// @param buffer The buffer of the variable binding.
/// @return A pointer to the variable binding on success. A null pointer on failure.
dx_vbinding* dx_context_create_vbinding(dx_context* context, DX_VERTEX_FORMAT vertex_format, dx_buffer* buffer);

/// Create a program.
/// @param context A pointer to this context.
/// @param program_text A program text of type #DX_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT.
/// @return A pointer to the program on success. A null pointer on failure.
dx_program* dx_context_create_program(dx_context* context, dx_program_text* program_text);

/// Execute a command list.
/// @param context A pointer to this context.
/// @param command_list A pointer to the command_list.
/// @return @a 0 on success. A non-zero value on failure.
int dx_context_execute_commands(dx_context* context, dx_command_list* commands);

#endif // DX_CONTEXT_H_INCLUDED
