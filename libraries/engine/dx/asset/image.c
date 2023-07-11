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

static void fill_rgb_u8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGB_U8 const* color);

static int on_color_fill_image_operation(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, dx_asset_image_operations_color_fill* image_operation);

static int on_checkerboard_pattern_fill_image_operation(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, dx_asset_image_operations_checkerboard_pattern_fill* image_operation);

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

static void fill_rgb_u8(void* pixels, OFFSET2 fill_offset, EXTEND2 fill_extend, EXTEND2 image_extend, DX_RGB_U8 const* color) {
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
    fill_rgb_u8(self->pixels, fill_offset, fill_size, image_size, color);
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
    fill_rgb_u8(self->pixels, offset, extend, image_size, &(image_operation->color));
  } break;
  default: {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  } break;
  };
  return 0;
}

static int on_checkerboard_pattern_fill_image_operation(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, dx_asset_image_operations_checkerboard_pattern_fill* image_operation) {
  dx_asset_image_operations_color_fill* first = dx_asset_image_operations_color_fill_create();
  dx_asset_image_operations_color_fill_set_color(first, image_operation->first_checker_color);
  dx_asset_image_operations_color_fill* second = dx_asset_image_operations_color_fill_create();
  dx_asset_image_operations_color_fill_set_color(second, image_operation->second_checker_color);
  if (dx_get_error()) {
    if (second) {
      DX_UNREFERENCE(second);
      second = NULL;
    }
    if (first) {
      DX_UNREFERENCE(first);
      first = NULL;
    }
  }
  for (dx_size y = 0; y < image_operation->number_of_checkers_vertical; ++y) {
    dx_size t = offset.top + y * image_operation->checker_size_vertical,
            h = image_operation->checker_size_vertical;
    // Fast clip.
    if (t + h > offset.top + extend.height) {
      continue;
    }
    // Slow clip.
    if (t + h > offset.top + extend.height) {
      dx_size delta = (t + h) - (offset.top + extend.height);
      h -= delta;
    }
    for (dx_size x = 0; x < image_operation->number_of_checkers_horizontal; ++x) {
      dx_size l = offset.left + x * image_operation->checker_size_horizontal,
              w = image_operation->checker_size_horizontal;
      // Fast clip.
      if (l > offset.left + extend.width) {
        continue;
      }
      // Slow clip.
      if (l + w > offset.left + extend.width) {
        dx_size delta = (l + w) - (offset.left + extend.width);
        w -= delta;
      }
      int even_x = x % 2 == 0;
      int even_y = y % 2 == 0;
      if (even_x != even_y) {
        dx_asset_image_apply(self, l, t, w, h, DX_ASSET_IMAGE_OPERATION(first));
      } else {
        dx_asset_image_apply(self, l, t, w, h, DX_ASSET_IMAGE_OPERATION(second));
      }
    }
  }
  DX_UNREFERENCE(second);
  second = NULL;
  DX_UNREFERENCE(first);
  first = NULL;
  return 0;
}

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
