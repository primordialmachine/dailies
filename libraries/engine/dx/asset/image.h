#if !defined(DX_ASSET_IMAGE_H_INCLUDED)
#define DX_ASSET_IMAGE_H_INCLUDED

#include "dx/core.h"
typedef struct dx_asset_image_operation dx_asset_image_operation;

/// @brief An image asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.image",
                       dx_asset_image,
                       dx_object)

static inline dx_asset_image* DX_ASSET_IMAGE(void* p) {
  return (dx_asset_image*)p;
}

/// @brief Construct this dx_asset_image_object with the specified pixel format, the specified width and height.
/// @param self A pointer to this dx_asset_image_object.
/// @param name A pointer to the ADL name of this image.
/// @param pixel_format The pixel format of the image.
/// @param width The width, in pixels, of the image. Can be @a 0.
/// @param height The height, in pixels, of the image. Can be @a 0.
/// @return
/// The zero value on success. A non-zero value on failure.
/// @default-failure
/// This function has set the error variable.
/// @remarks All pixels are assigned the default pixel value of the specified pixel format.
/// For DX_PIXEL_FORMAT_RGB this default value is the color "Black" (0, 0, 0).
int dx_asset_image_construct(dx_asset_image* self,
                             dx_string* name,
                             DX_PIXEL_FORMAT pixel_format,
                             dx_size width,
                             dx_size height,
                             DX_RGB_U8 const* color);

dx_asset_image* dx_asset_image_create(dx_string* name,
                                      DX_PIXEL_FORMAT pixel_format,
                                      dx_size width,
                                      dx_size height,
                                      DX_RGB_U8 const* color);

/// @brief Apply to the specified area the specified image operation.
/// @param self A pointer to this image.
/// @param left The left border of the area to fill.
/// @param top The top border of the area to fill.
/// @param width The width of the area to fill.
/// @param height The height of the area to fill.
/// @param image_operation A pointer to the image operation.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_image_apply(dx_asset_image* self,
                         dx_size left,
                         dx_size top,
                         dx_size width,
                         dx_size height,
                         dx_asset_image_operation* image_operation);

struct dx_asset_image {
  dx_object _parent;
  
  /// @brief A pointer to the name of this ADL image.
  dx_string* name;
  
  /// @brief The pixel format.
  DX_PIXEL_FORMAT pixel_format;
  
  /// @brief The width, in pixels.
  dx_size width;
  
  /// @brief The height, in pixels.
  dx_size height;
  
  /// @brief A pointer to the pixel data.
  void* pixels;

  /// @brief The operations that eventually create the image.
  dx_object_array operations;
};

#endif // DX_ASSET_IMAGE_H_INCLUDED
