#include "dx/asset/image.h"

// string
#include <string.h>
#include "dx/asset/image_operations/color_fill.h"
#include "dx/asset/image_operations/checkerboard_pattern_fill.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.image",
                      dx_asset_image,
                      dx_object)

typedef struct EXTEND2 {
  dx_size width;
  dx_size height;
} EXTEND2;

typedef struct OFFSET2 {
  dx_size left;
  dx_size top;
} OFFSET2;

static const DX_RGB_U8 black = { 0, 0, 0 };

static dx_size get_bytes_per_pixel(DX_PIXEL_FORMAT pixel_format);

// primitive operation
static int _swap_pixels_rgb_u8(dx_asset_image* self, dx_size source_x, dx_size source_y, dx_size target_x, dx_size target_y);

// primitive operation
static void _fill_rgb_u8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGB_U8 const* color);

/// @brief Swap two columns.
/// @param self A pointer to this image.
/// @param i, j The indices of the columns.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure This function has set the error variable.
/// @remark This is a non-primitive operation.
static int _swap_columns(dx_asset_image* self, dx_size i, dx_size j);

/// @brief Swap two rows.
/// @param self A pointer to this image.
/// @param i, j The indices of the rows.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// @remark This is a non-primitive operation.
static int _swap_rows(dx_asset_image* self, dx_size i, dx_size j);

static int on_color_fill_image_operation(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, dx_asset_image_operations_color_fill* image_operation);

static dx_size get_bytes_per_pixel(DX_PIXEL_FORMAT pixel_format) {
  switch (pixel_format) {
  case DX_PIXEL_FORMAT_RGB_U8: {
    return 3;
  } break;
  default: {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  } break;
  };
}

static void _fill_rgb_u8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGB_U8 const* color) {
  // fast clip
  if (fill_offset.left > image_extend.width) {
    return;
  }
  if (fill_offset.top > image_extend.height) {
    return;
  }
  dx_size fill_right = fill_offset.left + fill_extend.width;
  dx_size fill_bottom = fill_offset.top + fill_extend.height;
  // clamp
  if (fill_right > image_extend.width) {
    fill_right = image_extend.width;
  }
  if (fill_bottom > image_extend.height) {
    fill_bottom = image_extend.height;
  }
  if (fill_offset.left == fill_right) {
    return;
  }
  if (fill_offset.top == fill_bottom) {
    return;
  }
  dx_size bytes_per_pixel = get_bytes_per_pixel(DX_PIXEL_FORMAT_RGB_U8);
  for (dx_size y = fill_offset.top; y < fill_bottom; ++y) {
    for (dx_size x = fill_offset.left; x < fill_right; ++x) {
      dx_size offset_pixels = y * image_extend.width + x;
      dx_size offset_bytes = offset_pixels * bytes_per_pixel;
      *(DX_RGB_U8*)(((uint8_t*)pixels) + offset_bytes) = *color;
    }
  }
}

static int _swap_pixels_rgb_u8(dx_asset_image* self, dx_size source_x, dx_size source_y, dx_size target_x, dx_size target_y) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (source_x >= self->width || source_y >= self->height) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (target_x >= self->width || target_y >= self->height) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (source_x == target_x && source_y == target_y) {
    return 0;
  }
  dx_size bytes_per_pixel = get_bytes_per_pixel(DX_PIXEL_FORMAT_RGB_U8);

  dx_size source_offset_pixels = source_y * self->width + source_x;
  dx_size source_offset_bytes = source_offset_pixels * bytes_per_pixel;
  
  dx_size target_offset_pixels = target_y * self->height + target_x;
  dx_size target_offset_bytes = target_offset_pixels * bytes_per_pixel;

  DX_RGB_U8* source = (DX_RGB_U8*)((uint8_t*)self->pixels + source_offset_bytes);
  DX_RGB_U8* target = (DX_RGB_U8*)((uint8_t*)self->pixels + target_offset_bytes);

  DX_RGB_U8 temporary = *source;
  *source = *target;
  *target = temporary;

  return 0;
}

static int _swap_columns(dx_asset_image* self, dx_size i, dx_size j) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (i >= self->width || j >= self->width) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  switch (self->pixel_format) {
    case DX_PIXEL_FORMAT_RGB_U8: {
      // iterate over the y-axis
      for (dx_size y = 0; y < self->height; ++y) {
        _swap_pixels_rgb_u8(self, i, y, j, y);
      }
    } break;
    default: {
      dx_set_error(DX_NOT_IMPLEMENTED);
      return 1;
    } break;
  } 
  return dx_get_error();
}

static int _swap_rows(dx_asset_image* self, dx_size i, dx_size j) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (i >= self->height || j >= self->height) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  switch (self->pixel_format) {
    case DX_PIXEL_FORMAT_RGB_U8: {
      // iterate over the x-axis
      for (dx_size x = 0; x < self->width; ++x) {
        _swap_pixels_rgb_u8(self, x, i, x, j);
      }
    } break;
    default: {
      dx_set_error(DX_NOT_IMPLEMENTED);
      return 1;
    } break;
  }
  return dx_get_error();
}

static void dx_asset_image_destruct(dx_asset_image* self) {
  dx_object_array_uninitialize(&self->operations);
  DX_UNREFERENCE(self->name);
  self->name = NULL;
  dx_memory_deallocate(self->pixels);
  self->pixels = NULL;
}

int dx_asset_image_construct(dx_asset_image* self,
                             dx_string* name,
                             DX_PIXEL_FORMAT pixel_format,
                             dx_size width,
                             dx_size height,
                             DX_RGB_U8 const* color) {
  dx_rti_type* _type = dx_asset_image_get_type();
  if (!_type) {
    return 1;
  }
  self->width = width;
  self->height = height;
  self->pixel_format = pixel_format;
  dx_size overflow;
  dx_size number_of_pixels = dx_mul_sz(self->width, self->height, &overflow);
  if (overflow) {
    return 1;
  }
  dx_size bytes_per_pixel = get_bytes_per_pixel(pixel_format);
  if (dx_get_error()) {
    return 1;
  }
  dx_size number_of_bytes = dx_mul_sz(number_of_pixels, sizeof(uint8_t) * bytes_per_pixel, &overflow);
  if (overflow) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  self->pixels = dx_memory_allocate(number_of_bytes);
  if (!self->pixels) {
    return 1;
  }
  switch (self->pixel_format) {
  case DX_PIXEL_FORMAT_RGB_U8: {
    OFFSET2 fill_offset = { .left = 0, .top = 0 };
    EXTEND2 fill_size = { .width = self->width, .height = self->height };
    EXTEND2 image_size = { .width = self->width, .height = self->height };
    _fill_rgb_u8(self->pixels, fill_offset, fill_size, image_size, color);
  } break;
  default: {
    dx_memory_deallocate(self->pixels);
    self->pixels = NULL;
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  } break;
  };
  self->name = name;
  DX_REFERENCE(name);

  if (dx_object_array_initialize(&self->operations, 0)) {
    DX_UNREFERENCE(self->name);
    self->name = NULL;
    dx_memory_deallocate(self->pixels);
    self->pixels = NULL;
    return 1;
  }

  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_image* dx_asset_image_create(dx_string* name, 
                                      DX_PIXEL_FORMAT pixel_format,
                                      dx_size width,
                                      dx_size height,
                                      DX_RGB_U8 const* color) {
  dx_rti_type* _type = dx_asset_image_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_image* self = DX_ASSET_IMAGE(dx_object_alloc(sizeof(dx_asset_image)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_image_construct(self, name, pixel_format, width, height, color)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

static int on_color_fill_image_operation(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, dx_asset_image_operations_color_fill* image_operation) {
  switch (self->pixel_format) {
  case DX_PIXEL_FORMAT_RGB_U8: {
    EXTEND2 image_size = { .width = self->width, .height = self->height };
    _fill_rgb_u8(self->pixels, offset, extend, image_size, &(image_operation->color));
  } break;
  default: {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  } break;
  };
  return 0;
}

#include "dx/asset/image_operations/checkerboard_pattern_fill_impl.i"
#include "dx/asset/image_operations/mirror_horizontal_impl.i"
#include "dx/asset/image_operations/mirror_vertical_impl.i"

int dx_asset_image_apply(dx_asset_image* self,
                         dx_size left,
                         dx_size top,
                         dx_size width,
                         dx_size height,
                         dx_asset_image_operation* image_operation) {
  if (dx_rti_type_is_leq(DX_OBJECT(image_operation)->type, dx_asset_image_operations_color_fill_get_type())) {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_color_fill_image_operation(self, offset, extend, DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL(image_operation));
  }
  if (dx_get_error()) {
    return 1;
  }
  if (dx_rti_type_is_leq(DX_OBJECT(image_operation)->type, dx_asset_image_operations_checkerboard_pattern_fill_get_type())) {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_checkerboard_pattern_fill_image_operation(self, offset, extend, DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(image_operation));
  }
  if (dx_get_error()) {
    return 1;
  }
  dx_set_error(DX_INVALID_ARGUMENT);
  return 1;
}
