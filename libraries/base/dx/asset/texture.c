#include "dx/asset/texture.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.texture", dx_asset_texture, dx_object)

int dx_asset_texture_construct(dx_asset_texture* self, dx_asset_image* image) {
  if (!self || !image) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->image = image;
  DX_REFERENCE(image);
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_texture_destruct;
  return 0;
}

void dx_asset_texture_destruct(dx_asset_texture* self) {
  if (self->image) {
    DX_UNREFERENCE(self->image);
    self->image = NULL;
  }
}

dx_asset_texture* dx_asset_texture_create(dx_asset_image* image) {
  dx_asset_texture* self = DX_ASSET_TEXTURE(dx_object_alloc(sizeof(dx_asset_texture)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_texture_construct(self, image)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
