#if !defined(DX_TEXTURE_H_INCLUDED)
#define DX_TEXTURE_H_INCLUDED

#include "dx/val/context.h"
#include "dx/asset/texture.h"

DX_DECLARE_OBJECT_TYPE("dx.texture",
                       dx_texture,
                       dx_object)
  
static inline dx_texture* DX_TEXTURE(void* p) {
  return (dx_texture*)p;
}

struct dx_texture {
  dx_object _parent;
  dx_context* context;
  int (*set_data)(dx_texture* self, dx_asset_texture*);
};

int dx_texture_construct(dx_texture* self, dx_context* context);

/// @brief Set the data of this texture.
/// @param self A pointer to this texture.
/// @param texture A pointer to the texture asset.
/// @return The zero value on succcess. A non-zero value on failure.
/// @post This function sets the error variable on failure.
int dx_texture_set_data(dx_texture* self, dx_asset_texture* texture);

#endif // DX_TEXTURE_H_INCLUDED
