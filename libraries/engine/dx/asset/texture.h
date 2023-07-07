#if !defined(DX_ASSET_TEXTURE_H_INCLUDED)
#define DX_ASSET_TEXTURE_H_INCLUDED

#include "dx/asset/image.h"
#include "dx/asset/reference.h"

/// @brief A texture asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.texture",
                       dx_asset_texture,
                       dx_object)

static inline dx_asset_texture* DX_ASSET_TEXTURE(void* p) {
  return (dx_asset_texture*)p;
}

struct dx_asset_texture {
  dx_object _parent;
  /// @brief A pointer to the ADL name of this texture.
  dx_string* name;
  /// @brief The reference to the image of this texture.
  dx_asset_reference* image_reference;
};

/// @brief Construct this dx_asset_texture object.
/// @param self A pointer to this dx_asset_material object.
/// @param name A pointer to the ADL name of the texture.
/// @param image_reference The image reference of this texture.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
int dx_asset_texture_construct(dx_asset_texture* self, dx_string* name, dx_asset_reference* image_reference);

dx_asset_texture* dx_asset_texture_create(dx_string* name, dx_asset_reference* image_reference);

#endif // DX_ASSET_TEXTURE_H_INCLUDED
