#include "dx/val/viewer.h"

DX_DEFINE_OBJECT_TYPE("dx.val.viewer",
                      dx_val_viewer,
                      dx_object)

static void dx_val_viewer_destruct(dx_val_viewer* self) {
  DX_UNREFERENCE(self->asset_viewer_instance);
  self->asset_viewer_instance = NULL;
}

int dx_val_viewer_construct(dx_val_viewer* self, dx_asset_viewer_instance* asset_viewer_instance) {
  dx_rti_type* _type = dx_val_viewer_get_type();
  if (!_type) {
    return 1;
  }
  self->source = asset_viewer_instance->viewer->source;
  self->target = asset_viewer_instance->viewer->target;
  self->up = asset_viewer_instance->viewer->up;
  self->asset_viewer_instance = asset_viewer_instance;
  DX_REFERENCE(asset_viewer_instance);
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_val_viewer* dx_val_viewer_create(dx_asset_viewer_instance* asset_viewer_instance) {
  dx_val_viewer* self = DX_VAL_VIEWER(dx_object_alloc(sizeof(dx_val_viewer)));
  if (!self) {
    return NULL;
  }
  if (dx_val_viewer_construct(self, asset_viewer_instance)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
