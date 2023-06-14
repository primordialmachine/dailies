#include "dx/asset/material.h"

int dx_asset_material_construct(dx_asset_material* self) {
  self->ambient_color = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  self->ambient_texture = NULL;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_material_destruct;
  return 0;
}

void dx_asset_material_destruct(dx_asset_material* self) {
  if (self->ambient_texture) {
    DX_UNREFERENCE(self->ambient_texture);
    self->ambient_texture = NULL;
  }
}

dx_asset_material* dx_asset_material_create() {
  dx_asset_material* self = DX_ASSET_MATERIAL(dx_object_alloc(sizeof(dx_asset_material)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_material_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_asset_material_set_ambient_color(dx_asset_material* self, DX_VEC4 const* value) {
  if (!self || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->ambient_color = *value;
  return 0;
}

int dx_asset_material_set_ambient_texture(dx_asset_material* self, dx_asset_texture* value) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (value) {
    DX_REFERENCE(value);
  }
  if (self->ambient_texture) {
    DX_UNREFERENCE(self->ambient_texture);
  }
  self->ambient_texture = value;
  return 0;
}
