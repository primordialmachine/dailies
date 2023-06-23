#if !defined(DX_VBINDING_H_INCLUDED)
#define DX_VBINDING_H_INCLUDED

#include "dx/val/context.h"
typedef struct dx_buffer dx_buffer;

/// A "variable binding" (dx_vbinding) provides inputs to "variables" in "programs"
/// (like the "constant binding" (dx_cbinding) provides inputs to "constants" in "programs").
///
/// A variable binding owns a single buffer (dx_vbuffer) of raw Bytes and a description (DX_VSEMANTICS) how these raw Bytes are interpreted as data assigned to variables in programs.
/// Example:
/// A binding owns a buffer of sizeof(float)*(3+4)*4 Bytes.
/// Its description might say:
/// - interprete consecutive disjoint subsequences of sizeof(float)*(3+4) consecutive Bytes as a vertex
/// - the vertex consists of two vertex elements such that the first sizeof(float)*3 Bytes are interpreted as xyz position values and the last sizeof(float)*4 Bytes are interpreted as rgba color values.
/// - map the position vertex element to the vertex variable "position" in the program and the color vertex elements to the vertex variable of name "color" in the program
///
/// Currently, DX does not check if the specified variable buffer and the specified variable semantics are compatible when presenting:
/// That is, it is not verified that
/// - the buffer is big enough to provide th specified sub-sequence of variable value to present and
/// - the program has ariables of the right types declared to receive the variables values.
DX_DECLARE_OBJECT_TYPE("dx.vbinding", dx_vbinding, dx_object)

static inline dx_vbinding* DX_VBINDING(void* p) {
  return (dx_vbinding*)p;
}

struct dx_vbinding {
  dx_object _parent;
  dx_buffer* buffer;
  dx_context* context;
  int (*activate)(dx_vbinding*);
};

int dx_vbinding_construct(dx_vbinding* vbinding, dx_buffer* buffer);

void dx_vbinding_destruct(dx_vbinding* vbinding);

int dx_vbinding_activate(dx_vbinding* vbinding);

#endif // DX_VBINDING_H_INCLUDED
