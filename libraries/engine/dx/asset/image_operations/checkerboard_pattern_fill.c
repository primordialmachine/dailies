#include "dx/asset/image_operations/checkerboard_pattern_fill.h"

static void dx_asset_image_operations_checkerboard_pattern_fill_destruct(dx_asset_image_operations_checkerboard_pattern_fill* self)
{ }

DX_DEFINE_OBJECT_TYPE("dx.asset.image_operations.checkerboard_pattern_fill",
                      dx_asset_image_operations_checkerboard_pattern_fill,
                      dx_asset_image_operation)

int dx_asset_image_operations_checkerboard_pattern_fill_construct(dx_asset_image_operations_checkerboard_pattern_fill* self) {
  dx_rti_type* _type = dx_asset_image_operations_checkerboard_pattern_fill_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_asset_image_operation_construct(DX_ASSET_IMAGE_OPERATION(self))) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_image_operations_checkerboard_pattern_fill* dx_asset_image_operations_checkerboard_pattern_fill_create() {
  dx_rti_type* _type = dx_asset_image_operations_checkerboard_pattern_fill_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_image_operations_checkerboard_pattern_fill* self = DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(dx_object_alloc(sizeof(dx_asset_image_operations_checkerboard_pattern_fill)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_image_operations_checkerboard_pattern_fill_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}


#define PROPERTY(TYPE, PROPERTY_TYPE, PROPERTY_NAME) \
  int TYPE##_set_##PROPERTY_NAME(TYPE* self, PROPERTY_TYPE PROPERTY_NAME) { \
    self->PROPERTY_NAME = PROPERTY_NAME; \
    return 0; \
  } \
  int TYPE##_get_##PROPERTY_NAME##(PROPERTY_TYPE* result, TYPE* self) { \
    *result = self->PROPERTY_NAME; \
    return 0; \
  }

PROPERTY(dx_asset_image_operations_checkerboard_pattern_fill, dx_size, number_of_checkers_horizontal)
PROPERTY(dx_asset_image_operations_checkerboard_pattern_fill, dx_size, number_of_checkers_vertical)
PROPERTY(dx_asset_image_operations_checkerboard_pattern_fill, dx_size, checker_size_horizontal)
PROPERTY(dx_asset_image_operations_checkerboard_pattern_fill, dx_size, checker_size_vertical)

int dx_asset_image_operations_checkerboard_pattern_fill_set_first_checker_color(dx_asset_image_operations_checkerboard_pattern_fill* self, DX_RGB_U8* first_checker_color) {
  self->first_checker_color = *first_checker_color;
  return 0;
}

int dx_asset_image_operations_checkerboard_pattern_fill_get_first_checker_color(DX_RGB_U8* result, dx_asset_image_operations_checkerboard_pattern_fill* self) {
  *result = self->first_checker_color;
  return 0;
}

#undef PROPERTY

int dx_asset_image_operations_checkerboard_pattern_fill_set_second_checker_color(dx_asset_image_operations_checkerboard_pattern_fill* self, DX_RGB_U8* second_checker_color) {
  self->second_checker_color = *second_checker_color;
  return 0;
}

int dx_asset_image_operations_checkerboard_pattern_fill_get_second_checker_color(DX_RGB_U8* result, dx_asset_image_operations_checkerboard_pattern_fill* self) {
  *result = self->second_checker_color;
  return 0;
}
