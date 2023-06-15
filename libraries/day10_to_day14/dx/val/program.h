#if !defined(DX_PROGRAM_H_INCLUDED)
#define DX_PROGRAM_H_INCLUDED

#include "dx/val/context.h"
typedef struct dx_cbinding dx_cbinding;

typedef struct dx_program dx_program;

static inline dx_program* DX_PROGRAM(void* p) {
  return (dx_program*)p;
}

struct dx_program {
  dx_object _parent;
  dx_context* ctx;
  int (*activate)(dx_program*);
  int (*bind)(dx_program*, dx_cbinding*);
};

int dx_program_construct(dx_program* program, dx_context* ctx);

void dx_program_destruct(dx_program* program);

int dx_program_activate(dx_program* program);

/// @brief Apply the specified constant binding to this program.
/// @param program A pointer to this program.
/// @param binding A pointer to the specified constant binding.
/// @return @a 0 on success. A non-zero value on failure.
int dx_program_bind(dx_program* program, dx_cbinding* cbinding);

#endif // DX_PROGRAM_H_INCLUDED
