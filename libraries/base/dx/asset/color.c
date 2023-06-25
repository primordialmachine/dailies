#include "dx/asset/color.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.color", dx_asset_color, dx_object)

void dx_asset_color_destruct(dx_asset_color* self) { }

int dx_asset_color_construct(dx_asset_color* self, DX_RGB_U8 const* value) {
  if (!self || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->value = *value;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_color_destruct;
  return 0;
}

dx_asset_color* dx_asset_color_create(DX_RGB_U8 const* value) {
  dx_asset_color* self = DX_ASSET_COLOR(dx_object_alloc(sizeof(dx_asset_color)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_color_construct(self, value)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

DX_RGB_U8 const* dx_asset_color_get_value(dx_asset_color* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  return &self->value;
}
