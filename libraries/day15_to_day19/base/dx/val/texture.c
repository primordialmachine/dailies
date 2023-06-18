#include "dx/val/texture.h"

int dx_texture_construct(dx_texture* self, dx_context* context) {
  self->context = context;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_texture_destruct;
  return 0;
}

void dx_texture_destruct(dx_texture* self) {
  self->context = NULL;
}

int dx_texture_set_data(dx_texture* self, dx_asset_texture* texture) {
  return self->set_data(self, texture);
}
