#if !defined(DX_ASSET_IMAGE_H_INCLUDED)
#define DX_ASSET_IMAGE_H_INCLUDED

#include "dx/core.h"

/// @brief An image asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.image", dx_asset_image, dx_object)

static inline dx_asset_image* DX_ASSET_IMAGE(void* p) {
  return (dx_asset_image*)p;
}

/// @brief Construct this image with the specified pixel format, the specified width and height.
/// @param self A pointer to this image.
/// @param pixel_format The pixel format of the image.
/// @param width The width, in pixels, of the image. Can be @a 0.
/// @param height The height, in pixels, of the image. Can be @a 0.
/// @return
/// The zero value on success. A non-zero value on failure.
/// @failure
/// This function has set the error variable.
/// @remarks All pixels are assigned the default pixel value of the specified pixel format.
/// For DX_PIXEL_FORMAT_RGB this default value is the color "Black" (0, 0, 0).
int dx_asset_image_construct(dx_asset_image* self,
                             DX_PIXEL_FORMAT pixel_format,
                             dx_size width,
                             dx_size height,
                             DX_RGB_U8 const* color);

void dx_asset_image_destruct(dx_asset_image* self);

dx_asset_image* dx_asset_image_create(DX_PIXEL_FORMAT pixel_format,
                                      dx_size width,
                                      dx_size height,
                                      DX_RGB_U8 const* color);

// A brush that fills its area with a single color.
#define DX_ASSET_BRUSH_FLAGS_SOLID (0)
// A brush that fills its area with a checkerboard pattern.
#define DX_ASSET_BRUSH_FLAGS_CHECKBERBOARD (1)

typedef struct DX_ASSET_BRUSH {
  uint8_t flags;
} DX_ASSET_BRUSH;

typedef struct DX_ASSET_SOLID_BRUSH {
  DX_ASSET_BRUSH _parent;
  DX_RGB_U8 color;
} DX_ASSET_SOLID_BRUSH;

typedef struct DX_ASSET_CHECKERBOARD_BRUSH {
  DX_ASSET_BRUSH _parent;
  struct {
    dx_size horizontal;
    dx_size vertical;
  } number_of_checkers;
  struct {
    dx_size horizontal;
    dx_size vertical;
  } checker_size;
  struct {
    DX_RGB_U8 first;
    DX_RGB_U8 second;
  } checker_colors;
} DX_ASSET_CHECKERBOARD_BRUSH;

/// @brief Fill the specified area of this image using the specified brush.
/// @param self A pointer to this image.
/// @param left The left border of the area to fill.
/// @param top The top border of the area to fill.
/// @param width The width of the area to fill.
/// @param height The height of the area to fill.
/// @param brush A pointer to the brush.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_image_fill(dx_asset_image* self,
                        dx_size left,
                        dx_size top,
                        dx_size width,
                        dx_size height,
                        DX_ASSET_BRUSH const* brush);

struct dx_asset_image {
  dx_object _parent;
  /// @brief The pixel format.
  DX_PIXEL_FORMAT pixel_format;
  /// @brief The width, in pixels.
  dx_size width;
  /// @brief The height, in pixels.
  dx_size height;
  /// @brief A pointer to the pixel data.
  void* pixels;
};

#endif // DX_ASSET_IMAGE_H_INCLUDED