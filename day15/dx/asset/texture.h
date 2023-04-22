#if !defined(DX_ASSET_TEXTURE_H_INCLUDED)
#define DX_ASSET_TEXTURE_H_INCLUDED

#include "dx/asset/image.h"

/// @brief Representation of a texture asset.
typedef struct dx_asset_texture dx_asset_texture;
static inline dx_asset_texture* DX_ASSET_TEXTURE(void* p) {
  return (dx_asset_texture*)p;
}

struct dx_asset_texture {
  dx_object _parent;
  dx_asset_image* image;
};

int dx_asset_texture_construct(dx_asset_texture* self, dx_asset_image* image);

void dx_asset_texture_destruct(dx_asset_texture* texture);

dx_asset_texture* dx_asset_texture_create(dx_asset_image* image);

#endif // DX_ASSET_TEXTURE_H_INCLUDED
