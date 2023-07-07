#include "dx/asset/reference.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.reference",
                      dx_asset_reference,
                      dx_object)

static void dx_asset_reference_destruct(dx_asset_reference* self) {
  if (self->object) {
    DX_UNREFERENCE(self->object);
    self->object = NULL;
  }
  DX_UNREFERENCE(self->name);
  self->name = NULL;
}


int dx_asset_reference_construct(dx_asset_reference* self, dx_string* name) {
  dx_rti_type* _type = dx_asset_reference_get_type();
  if (!_type) {
    return 1;
  }
  if (!self || !name) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->name = name;
  DX_REFERENCE(self->name);
  self->object = NULL;
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_reference* dx_asset_reference_create(dx_string* name) {
  dx_rti_type* _type = dx_asset_reference_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_reference* self = DX_ASSET_REFERENCE(dx_object_alloc(sizeof(dx_asset_reference)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_reference_construct(self, name)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
