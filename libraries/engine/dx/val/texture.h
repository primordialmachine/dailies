#if !defined(DX_VAL_TEXTURE_H_INCLUDED)
#define DX_VAL_TEXTURE_H_INCLUDED

#include "dx/val/context.h"
#include "dx/asset/texture.h"

DX_DECLARE_OBJECT_TYPE("dx.val.texture",
                       dx_val_texture,
                       dx_val_object)
  
static inline dx_val_texture* DX_VAL_TEXTURE(void* p) {
  return (dx_val_texture*)p;
}

struct dx_val_texture {
  dx_object _parent;
  dx_context* context;
  int (*set_data)(dx_val_texture* self, dx_asset_texture*);
};

int dx_val_texture_construct(dx_val_texture* self, dx_context* context);

/// @brief Set the data of this texture.
/// @param self A pointer to this texture.
/// @param texture A pointer to the texture asset.
/// @return The zero value on succcess. A non-zero value on failure.
/// @post This function sets the error variable on failure.
int dx_val_texture_set_data(dx_val_texture* self, dx_asset_texture* texture);

#endif // DX_VAL_TEXTURE_H_INCLUDED
