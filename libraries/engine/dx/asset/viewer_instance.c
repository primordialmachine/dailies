#include "dx/asset/viewer_instance.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.viewer_instance",
                      dx_asset_viewer_instance,
                      dx_object)

int dx_asset_viewer_instance_construct(dx_asset_viewer_instance* self, dx_asset_viewer* viewer) {
  dx_rti_type* _type = dx_asset_viewer_instance_get_type();
  if (!_type) {
    return 1;
  }
  if (!self || !viewer) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->viewer = viewer;
  DX_REFERENCE(self->viewer);
  dx_mat4_set_identity(&self->world_matrix);
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_asset_viewer_instance_destruct(dx_asset_viewer_instance* self) {
  DX_UNREFERENCE(self->viewer);
  self->viewer = NULL;
}

dx_asset_viewer_instance* dx_asset_viewer_instance_create(dx_asset_viewer* viewer) {
  dx_asset_viewer_instance* self = DX_ASSET_VIEWER_INSTANCE(dx_object_alloc(sizeof(dx_asset_viewer_instance)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_viewer_instance_construct(self, viewer)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
