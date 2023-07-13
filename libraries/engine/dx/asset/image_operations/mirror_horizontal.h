#if !defined(DX_ASSET_IMAGE_OPERATIONS_MIRROR_HORIZONTAL_H_INCLUDED)
#define DX_ASSET_IMAGE_OPERATIONS_MIRROR_HORIZONTAL_H_INCLUDED

#include "dx/asset/image_operation.h"
#include "dx/asset/reference.h"

/// @extends dx_asset_image_operation
/// @brief An image operation.
/// Mirrors an image horizontaly.
/// Let \f$w geq 0\f$ and \f$h \geq 0\f$ be the width of the height of the image, respectively,
/// then the pixel at \f$(x,y)\f$ with \f$x \in [1,w]\f$ and \f$y \in [1,h]\f$ is swapped with the pixel at \f$(w-x,y)\f$. 
DX_DECLARE_OBJECT_TYPE("dx.asset.image_operations.mirror_horizontal",
                       dx_asset_image_operations_mirror_horizontal,
                       dx_asset_image_operation)

static inline dx_asset_image_operations_mirror_horizontal* DX_ASSET_IMAGE_OPERATIONS_MIRROR_HORIZONTAL(void* p) {
  return (dx_asset_image_operations_mirror_horizontal*)p;
}

struct dx_asset_image_operations_mirror_horizontal {
  dx_asset_image_operation _parent;
};

/// @brief Construct this dx_asset_image_operations_mirror_horizontal object with default values.
/// @param self A pointer to this dx_asset_image_operations_mirror_horizontal object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_mirror_horizontal_construct(dx_asset_image_operations_mirror_horizontal* self);

dx_asset_image_operations_mirror_horizontal* dx_asset_image_operations_mirror_horizontal_create();

/// @brief Set the color.
/// @param self A pointer to this dx_asset_image_operations_mirror_horizontal object.
/// @param color A pointer to the DX_RGB_U8 color object providing the color values.
/// @default-return
/// @default-failure
int dx_asset_image_operations_mirror_horizontal_set_color(dx_asset_image_operations_mirror_horizontal* self, dx_asset_color* color);

/// @brief Get the color.
/// @param result A pointer to the DX_RGB_U8 color object receiving the color values.
/// @param self A pointer to this dx_asset_image_operations_mirror_horizontal object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_mirror_horizontal_get_color(DX_RGB_U8* result, dx_asset_image_operations_mirror_horizontal* self);

#endif // DX_ASSET_IMAGE_OPERATIONS_MIRROR_HORIZONTAL_H_INCLUDED
