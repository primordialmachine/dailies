#if !defined(DX_CBINDING_H_INCLUDED)
#define DX_CBINDING_H_INCLUDED

#include "dx/core.h"

/// A "constant binding" (dx_cbinding) provides inputs to "constants" in "programs"
/// (like the "variable binding" (dx_vbinding) provides inputs to "variables" in "programs").
DX_DECLARE_OBJECT_TYPE("dx.cbinding", dx_cbinding, dx_object)

static inline dx_cbinding* DX_CBINDING(void* p) {
  return (dx_cbinding*)p;
}

struct dx_cbinding {
  dx_object _parent;
  void* pimpl;
};

#define DX_CBINDING_TYPE_EMPTY (0)
#define DX_CBINDING_TYPE_MAT4 (1)
#define DX_CBINDING_TYPE_VEC3 (2)
#define DX_CBINDING_TYPE_VEC4 (3)
#define DX_CBINDING_TYPE_TEXTURE_INDEX (4)

int dx_cbinding_set_vec3(dx_cbinding* cbinding, char const* name, DX_VEC3 const* v);

int dx_cbinding_set_vec4(dx_cbinding* cbinding, char const* name, DX_VEC4 const* v);

int dx_cbinding_set_mat4(dx_cbinding* cbinding, char const* name, DX_MAT4 const* a);

int dx_cbinding_set_texture_index(dx_cbinding* cbinding, char const* name, size_t i);

int dx_cbinding_construct(dx_cbinding* cbinding);

void dx_cbinding_destruct(dx_cbinding* cbinding);

dx_cbinding* dx_cbinding_create();

typedef struct dx_cbinding_iter {
  void* a;
  void* b;
  void* c;
} dx_cbinding_iter;

dx_cbinding_iter dx_cbinding_get_iter(dx_cbinding* cbinding);

int dx_cbinding_iter_next(dx_cbinding_iter* iter);

/// @return @a true if there is a value. @a false if there is no value. @a false is also returned on failure.
bool dx_cbinding_iter_has_value(dx_cbinding_iter const* iter);

/// @return The tag on success. DX_CBINDING_TYPE_EMPTY is also returned on failure.
uint8_t dx_cbinding_iter_get_tag(dx_cbinding_iter const* iter);

/// @return A pointer to the name on success. The null poiter on failure.
char const* dx_cbinding_iter_get_name(dx_cbinding_iter const* iter);

int dx_cbinding_iter_get_vec3(dx_cbinding_iter const* iter, DX_VEC3* v);

int dx_cbinding_iter_get_vec4(dx_cbinding_iter const* iter, DX_VEC4* v);

int dx_cbinding_iter_get_mat4(dx_cbinding_iter const* iter, DX_MAT4* a);

int dx_cbinding_iter_get_texture_index(dx_cbinding_iter const* iter, size_t *i);

#endif // DX_CBINDING_H_INCLUDED
