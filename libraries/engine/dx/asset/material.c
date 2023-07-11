#include "dx/asset/material.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.material",
                      dx_asset_material,
                      dx_object)

static void dx_asset_material_destruct(dx_asset_material* self) {
  if (self->ambient_texture_reference) {
    DX_UNREFERENCE(self->ambient_texture_reference);
    self->ambient_texture_reference = NULL;
  }
  if (self->name) {
    DX_UNREFERENCE(self->name);
    self->name = NULL;
  }
}

int dx_asset_material_construct(dx_asset_material* self, dx_string* name) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_asset_material_get_type();
  if (!_type) {
    return 1;
  }
  
  if (!name) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->name = name;
  DX_REFERENCE(self->name);

  DX_RGB_U8 WHITE = { .r = 255, .g = 255, .b = 255 };
  self->ambient_color = dx_asset_color_create(&WHITE);
  if (!self->ambient_color) {
    DX_UNREFERENCE(self->name);
    self->name = NULL;
    return 1;
  }
  
  self->ambient_texture_reference = NULL;
  
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_material* dx_asset_material_create(dx_string* name) {
  dx_asset_material* self = DX_ASSET_MATERIAL(dx_object_alloc(sizeof(dx_asset_material)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_material_construct(self, name)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_asset_material_set_ambient_color(dx_asset_material* self, dx_asset_color* ambient_color) {
  if (!self || !ambient_color) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  DX_REFERENCE(ambient_color);
  DX_UNREFERENCE(self->ambient_color);
  self->ambient_color = ambient_color;
  return 0;
}

int dx_asset_material_set_ambient_texture(dx_asset_material* self, dx_asset_reference* ambient_texture_reference) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (ambient_texture_reference) {
    DX_REFERENCE(ambient_texture_reference);
  }
  if (self->ambient_texture_reference) {
    DX_UNREFERENCE(self->ambient_texture_reference);
  }
  self->ambient_texture_reference = ambient_texture_reference;
  return 0;
}
