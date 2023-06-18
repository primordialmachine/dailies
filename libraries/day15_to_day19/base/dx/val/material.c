#include "dx/val/material.h"

#include "dx/val/texture.h"

static int add_to_backend(dx_material* self) {
  if (self->asset_material->ambient_texture) {
    dx_texture* ambient_texture = dx_context_create_texture(self->context);
    if (!ambient_texture) {
      return 1;
    }
    if (dx_texture_set_data(ambient_texture, self->asset_material->ambient_texture)) {
      DX_UNREFERENCE(ambient_texture);
      ambient_texture = NULL;
      return 1;
    }
    self->ambient_texture = ambient_texture;
  }
  return 0;
}

static void remove_from_backend(dx_material* self) {
  if (self->ambient_texture) {
    DX_UNREFERENCE(self->ambient_texture);
    self->ambient_texture = NULL;
  }
}

int dx_material_construct(dx_material* self, dx_context* context, dx_asset_material* asset_material) {
  if (!self || !context || !asset_material) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->context = context;
  self->asset_material = asset_material;
  DX_REFERENCE(asset_material);
  self->ambient_color = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  self->ambient_texture = NULL;
  if (add_to_backend(self)) {
    DX_UNREFERENCE(self->asset_material);
    self->asset_material = NULL;
    return 1;
  }
  DX_OBJECT(self)->destruct = (void (*)(dx_object*)) & dx_material_destruct;
  return 0;
}

void dx_material_destruct(dx_material* self) {
  remove_from_backend(self);
  DX_UNREFERENCE(self->asset_material);
  self->asset_material = NULL;
}

dx_material* dx_material_create(dx_context* context, dx_asset_material* asset_material) {
  dx_material* self = DX_MATERIAL(dx_object_alloc(sizeof(dx_material)));
  if (!self) {
    return NULL;
  }
  if (dx_material_construct(self, context, asset_material)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
