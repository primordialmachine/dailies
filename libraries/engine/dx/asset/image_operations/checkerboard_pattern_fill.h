#if !defined(DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED)
#define DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED

#include "dx/asset/image_operation.h"
#include "dx/asset/reference.h"

/// @extends dx_asset_image_operation
/// @brief An image operation.
/// Fills an image with the a checkerboard pattern.
DX_DECLARE_OBJECT_TYPE("dx.asset.image_operations.checkerboard_pattern_fill",
                       dx_asset_image_operations_checkerboard_pattern_fill,
                       dx_asset_image_operation)

static inline dx_asset_image_operations_checkerboard_pattern_fill* DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(void* p) {
  return (dx_asset_image_operations_checkerboard_pattern_fill*)p;
}

struct dx_asset_image_operations_checkerboard_pattern_fill {
  dx_asset_image_operation _parent;
  
  dx_size number_of_checkers_horizontal;
  dx_size number_of_checkers_vertical;

  dx_size checker_size_horizontal;
  dx_size checker_size_vertical;

  DX_RGB_U8 first_checker_color;
  DX_RGB_U8 second_checker_color;
};

/// @brief Construct this dx_asset_image_operations_checkerboard_pattern_fill object with default values.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_construct(dx_asset_image_operations_checkerboard_pattern_fill* self);

dx_asset_image_operations_checkerboard_pattern_fill* dx_asset_image_operations_checkerboard_pattern_fill_create();



/// @brief Set the number of checkers along the horizontal axis.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_horizontal(dx_asset_image_operations_checkerboard_pattern_fill* self, dx_size number_of_checkers_horizontal);

/// @brief Get the number of checkers along the horizontal axis.
/// @param result A pointer to a <code>dx_size</code> variable receiving the number of checkers along the vertical axis.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_get_number_of_checkers_horizontal(dx_size* result, dx_asset_image_operations_checkerboard_pattern_fill* self);



/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_vertical(dx_asset_image_operations_checkerboard_pattern_fill* self, dx_size number_of_checkers_vertical);

/// @brief Get the number of checkers along the vertical axis.
/// @param result A pointer to a <code>dx_size</code> variable receiving the number of checkers along the vertical axis.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_get_number_of_checkers_vertical(dx_size* result, dx_asset_image_operations_checkerboard_pattern_fill* self);



/// @brief Set the size of a checker along the horizontal axis.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @param checker_size_horizontal The size of a checker along the horizontal axis.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_horizontal(dx_asset_image_operations_checkerboard_pattern_fill* self, dx_size checker_size_horizontal);

/// @brief Get the size of a checker along the horizontal axis.
/// @param result A pointer to the dx_size variable receiving the size of a checker along the horizontal axis.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_get_checker_size_horizontal(dx_size* result, dx_asset_image_operations_checkerboard_pattern_fill* self);



/// @brief Set the size of a checker along the vertical axis.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @param checker_size_vertical The size of a checker along the vertical axis.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_vertical(dx_asset_image_operations_checkerboard_pattern_fill* self, dx_size checker_size_vertical);

/// @brief Get the size of a checker along the vertical axis.
/// @param result A pointer to the dx_size variable receiving the size of a checker along the vertical axis.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_get_checker_size_vertical(dx_size* result, dx_asset_image_operations_checkerboard_pattern_fill* self);



/// @brief Set the first checker color.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @param first_checker_color A pointer to the DX_RGB_U8 color object providing the color values.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_set_first_checker_color(dx_asset_image_operations_checkerboard_pattern_fill* self, DX_RGB_U8* first_checker_color);

/// @brief Get the first checker color.
/// @param result A pointer to the DX_RGB_U8 color object receiving the color values.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_get_first_checker_color(DX_RGB_U8* result, dx_asset_image_operations_checkerboard_pattern_fill* self);




/// @brief Set the second checker color.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @param second_checker_color A pointer to the DX_RGB_U8 color object providing the color values.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_set_second_checker_color(dx_asset_image_operations_checkerboard_pattern_fill* self, DX_RGB_U8* second_checker_color);

/// @brief Get the first checker color.
/// @param result A pointer to the DX_RGB_U8 color object receiving the color values.
/// @param self A pointer to this dx_asset_image_operations_checkerboard_pattern_fill object.
/// @default-return
/// @default-failure
int dx_asset_image_operations_checkerboard_pattern_fill_get_second_checker_color(DX_RGB_U8* result, dx_asset_image_operations_checkerboard_pattern_fill* self);

#endif // DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED
