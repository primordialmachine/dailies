#if !defined(DX_ASSET_COLOR_H_INCLUDED)
#define DX_ASSET_COLOR_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.asset.color", dx_asset_color, dx_object)

static inline dx_asset_color* DX_ASSET_COLOR(void* p) {
  return (dx_asset_color*)p;
}

struct dx_asset_color {
  dx_object _parent;
  DX_RGB_U8 value;
};

int dx_asset_color_construct(dx_asset_color* self, DX_RGB_U8 const* value);

dx_asset_color* dx_asset_color_create(DX_RGB_U8 const* value);

/// @brief Get a pointer to the underlaying value.
/// @param self A pointer to this color.
/// @return A pointer to the underlaying value on success. The null pointer on failure.
DX_RGB_U8 const* dx_asset_color_get_value(dx_asset_color* self);

#endif // DX_ASSET_COLOR_H_INCLUDED
