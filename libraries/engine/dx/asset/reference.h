#if !defined(DX_ASSET_REFERENCE_H_INCLUDED)
#define DX_ASSET_REFERENCE_H_INCLUDED

#include "dx/core.h"

/// @brief A reference to another definition.
/// For example
/// @code
/// { ... color : "malachite" ... }
/// @endcode
/// is a reference to an object of name @code{malachite}.
DX_DECLARE_OBJECT_TYPE("dx.asset.reference",
                       dx_asset_reference,
                       dx_object)

static inline dx_asset_reference* DX_ASSET_REFERENCE(void* p) {
  return (dx_asset_reference*)p;
}

struct dx_asset_reference {
  dx_object _parent;
  /// @brief The name.
  dx_string* name;
  /// @brief A pointer to the object if it was successfully resolved.
  dx_object* object;
};

int dx_asset_reference_construct(dx_asset_reference* self, dx_string* name);

dx_asset_reference* dx_asset_reference_create(dx_string* name);

#endif // DX_ASSET_REFERENCE_H_INCLUDED
