#include "dx/val/material.h"

#include "dx/val/texture.h"

DX_DEFINE_OBJECT_TYPE("dx.val.material",
                      dx_val_material,
                      dx_object)

static int add_to_backend(dx_val_material* self) {
  if (self->asset_material->ambient_texture_reference && self->asset_material->ambient_texture_reference->object) {
    dx_asset_texture* asset_ambient_texture = DX_ASSET_TEXTURE(self->asset_material->ambient_texture_reference->object);
    dx_val_texture* ambient_texture = dx_context_create_texture(self->context);
    if (!ambient_texture) {
      return 1;
    }
    if (dx_val_texture_set_data(ambient_texture, asset_ambient_texture)) {
      DX_UNREFERENCE(ambient_texture);
      ambient_texture = NULL;
      return 1;
    }
    self->ambient_texture = ambient_texture;
  }
  return 0;
}

static void remove_from_backend(dx_val_material* self) {
  if (self->ambient_texture) {
    DX_UNREFERENCE(self->ambient_texture);
    self->ambient_texture = NULL;
  }
}

int dx_val_material_construct(dx_val_material* self, dx_context* context, dx_asset_material* asset_material) {
  if (!self || !context || !asset_material) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_val_material_get_type();
  if (!_type) {
    return 1;
  }
  self->context = context;
  self->asset_material = asset_material;
  DX_REFERENCE(asset_material);
  DX_VEC4 temporary;
  temporary.e[0] = ((dx_f32)asset_material->ambient_color->value.r) / 255.f;
  temporary.e[1] = ((dx_f32)asset_material->ambient_color->value.g) / 255.f;
  temporary.e[2] = ((dx_f32)asset_material->ambient_color->value.b) / 255.f;
  temporary.e[3] = 1.f;
  self->ambient_color = temporary;
  self->ambient_texture = NULL;
  if (add_to_backend(self)) {
    DX_UNREFERENCE(self->asset_material);
    self->asset_material = NULL;
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_val_material_destruct(dx_val_material* self) {
  remove_from_backend(self);
  DX_UNREFERENCE(self->asset_material);
  self->asset_material = NULL;
}

dx_val_material* dx_val_material_create(dx_context* context, dx_asset_material* asset_material) {
  dx_val_material* self = DX_VAL_MATERIAL(dx_object_alloc(sizeof(dx_val_material)));
  if (!self) {
    return NULL;
  }
  if (dx_val_material_construct(self, context, asset_material)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
