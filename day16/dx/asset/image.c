#include "dx/asset/image.h"

// malloc, free
#include <malloc.h>
// string
#include <string.h>

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

static int on_solid(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, DX_ASSET_SOLID_BRUSH const* brush);

static int on_checkerboard(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, DX_ASSET_CHECKERBOARD_BRUSH const* brush);

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

int dx_asset_image_construct(dx_asset_image* self,
                             DX_PIXEL_FORMAT pixel_format,
                             dx_size width,
                             dx_size height,
                             DX_RGB_U8 const* color) {
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
    return 1;
  }
  self->pixels = malloc(number_of_bytes);
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
    free(self->pixels);
    self->pixels = NULL;
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  } break;
  };
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_image_destruct;
  return 0;
}

void dx_asset_image_destruct(dx_asset_image* self) {
  free(self->pixels);
  self->pixels = NULL;
}

dx_asset_image* dx_asset_image_create(DX_PIXEL_FORMAT pixel_format,
                                      dx_size width,
                                      dx_size height,
                                      DX_RGB_U8 const* color) {
  dx_asset_image* self = DX_ASSET_IMAGE(dx_object_alloc(sizeof(dx_asset_image)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_image_construct(self, pixel_format, width, height, color)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

static int on_solid(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, DX_ASSET_SOLID_BRUSH const* brush) {
  switch (self->pixel_format) {
  case DX_PIXEL_FORMAT_RGB_U8: {
    EXTEND2 image_size = { .width = self->width, .height = self->height };
    fill_rgb_u8(self->pixels, offset, extend, image_size, &(brush->color));
  } break;
  default: {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  } break;
  };
  return 0;
}

static int on_checkerboard(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, DX_ASSET_CHECKERBOARD_BRUSH const* brush) {
  for (dx_size y = 0; y < brush->number_of_checkers.vertical; ++y) {
    dx_size t = offset.top + y * brush->checker_size.vertical,
           h = brush->checker_size.vertical;
    // Fast clip.
    if (t + h > offset.top + extend.height) {
      continue;
    }
    // Slow clip.
    if (t + h > offset.top + extend.height) {
      dx_size delta = (t + h) - (offset.top + extend.height);
      h -= delta;
    }
    for (dx_size x = 0; x < brush->number_of_checkers.horizontal; ++x) {
      dx_size l = offset.left + x * brush->checker_size.horizontal,
             w = brush->checker_size.horizontal;
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
      DX_ASSET_SOLID_BRUSH brush_1;
      brush_1._parent.flags = DX_ASSET_BRUSH_FLAGS_SOLID;
      if (even_x != even_y) {
        brush_1.color = brush->checker_colors.first;
      } else {
        brush_1.color = brush->checker_colors.second;
      }
      dx_asset_image_fill(self, l, t, w, h, (DX_ASSET_BRUSH const*)&brush_1);
    }
  }
  return 0;
}

int dx_asset_image_fill(dx_asset_image* self,
                        dx_size left,
                        dx_size top,
                        dx_size width,
                        dx_size height,
                        DX_ASSET_BRUSH const* brush) {
  switch (brush->flags) {
  case DX_ASSET_BRUSH_FLAGS_SOLID: {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_solid(self, offset, extend, (DX_ASSET_SOLID_BRUSH const*)brush);
  } break;
  case DX_ASSET_BRUSH_FLAGS_CHECKBERBOARD: {
    OFFSET2 offset = { .left = left, .top = top };
    EXTEND2 extend = { .width = width, .height = height };
    return on_checkerboard(self, offset, extend, (DX_ASSET_CHECKERBOARD_BRUSH const*)brush);
  } break;
  default: {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  } break;
  };
}
