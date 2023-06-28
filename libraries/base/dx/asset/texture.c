#include "dx/asset/texture.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.texture", dx_asset_texture, dx_object)

int dx_asset_texture_construct(dx_asset_texture* self, dx_string* name, dx_asset_image* image) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_asset_texture_get_type();
  if (!_type) {
    return 1;
  }
  if (!name || !image) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  
  self->image = image;
  DX_REFERENCE(image);

  self->name = name;
  DX_REFERENCE(name);

  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_asset_texture_destruct(dx_asset_texture* self) {
  if (self->name) {
    DX_UNREFERENCE(self->name);
    self->name = NULL;
  }
  if (self->image) {
    DX_UNREFERENCE(self->image);
    self->image = NULL;
  }
}

dx_asset_texture* dx_asset_texture_create(dx_string* name, dx_asset_image* image) {
  dx_asset_texture* self = DX_ASSET_TEXTURE(dx_object_alloc(sizeof(dx_asset_texture)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_texture_construct(self, name, image)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
