#include "dx/val/texture.h"

DX_DEFINE_OBJECT_TYPE("dx.val.texture",
                      dx_val_texture,
                      dx_object)

int dx_val_texture_construct(dx_val_texture* self, dx_context* context) {
  dx_rti_type* _type = dx_val_texture_get_type();
  if (!_type) {
    return 1;
  }
  self->context = context;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_val_texture_destruct(dx_val_texture* self) {
  self->context = NULL;
}

int dx_val_texture_set_data(dx_val_texture* self, dx_asset_texture* texture) {
  return self->set_data(self, texture);
}
