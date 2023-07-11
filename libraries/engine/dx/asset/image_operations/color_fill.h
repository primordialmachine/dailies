#if !defined(DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED)
#define DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED

#include "dx/asset/image_operation.h"
#include "dx/asset/reference.h"

/// @extends dx_asset_image_operation
/// @brief An image operation.
/// Fills an image with a color.
DX_DECLARE_OBJECT_TYPE("dx.asset.color_fill",
                       dx_asset_image_operations_color_fill,
                       dx_asset_image_operation)

static inline dx_asset_image_operations_color_fill* DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL(void* p) {
  return (dx_asset_image_operations_color_fill*)p;
}

struct dx_asset_image_operations_color_fill {
  dx_asset_image_operation _parent;
  DX_RGB_U8 color;
};

/// @brief Construct this dx_asset_image_operations_color_fill object with default values.
/// @param self A pointer to this dx_asset_image_operations_color_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_color_fill_construct(dx_asset_image_operations_color_fill* self);

dx_asset_image_operations_color_fill* dx_asset_image_operations_color_fill_create();

/// @brief Set the color.
/// @param self A pointer to this dx_asset_image_operations_color_fill object.
/// @param color A pointer to the DX_RGB_U8 color object providing the color values.
/// @default-return
/// @default-failure
int dx_asset_image_operations_color_fill_set_color(dx_asset_image_operations_color_fill* self, dx_asset_color* color);

/// @brief Get the color.
/// @param result A pointer to the DX_RGB_U8 color object receiving the color values.
/// @param self A pointer to this dx_asset_image_operations_color_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_color_fill_get_color(DX_RGB_U8* result, dx_asset_image_operations_color_fill* self);

#endif // DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED
