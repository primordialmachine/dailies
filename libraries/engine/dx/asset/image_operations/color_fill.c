#include "dx/asset/image_operations/color_fill.h"

static void dx_asset_image_operations_color_fill_destruct(dx_asset_image_operations_color_fill* self)
{ }

DX_DEFINE_OBJECT_TYPE("dx.asset.image_operations_color_fill",
                      dx_asset_image_operations_color_fill,
                      dx_asset_image_operation)

int dx_asset_image_operations_color_fill_construct(dx_asset_image_operations_color_fill* self) {
  dx_rti_type* _type = dx_asset_image_operations_color_fill_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_asset_image_operation_construct(DX_ASSET_IMAGE_OPERATION(self))) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_image_operations_color_fill* dx_asset_image_operations_color_fill_create() {
  dx_rti_type* _type = dx_asset_image_operations_color_fill_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_image_operations_color_fill* self = DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL(dx_object_alloc(sizeof(dx_asset_image_operations_color_fill)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_image_operations_color_fill_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_asset_image_operations_color_fill_set_color(dx_asset_image_operations_color_fill* self, DX_RGB_U8* color) {
  self->color = *color;
  return 0;
}

int dx_asset_image_operations_color_fill_get_color(DX_RGB_U8* result, dx_asset_image_operations_color_fill* self) {
  *result = self->color;
  return 0;
}
