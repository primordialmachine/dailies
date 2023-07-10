#include "dx/asset/viewer_instance.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.viewer_instance",
                      dx_asset_viewer_instance,
                      dx_object)

static void dx_asset_viewer_instance_destruct(dx_asset_viewer_instance* self) {
  DX_UNREFERENCE(self->viewer_reference);
  self->viewer_reference = NULL;
}

int dx_asset_viewer_instance_construct(dx_asset_viewer_instance* self, dx_asset_viewer* viewer) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_asset_viewer_instance_get_type();
  if (!_type) {
    return 1;
  }

  if (!viewer) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->viewer_reference = dx_asset_reference_create(viewer->name);
  if (!self->viewer_reference) {
    return 1;
  }
  self->viewer_reference->object = DX_OBJECT(viewer);
  DX_REFERENCE(self->viewer_reference->object);
  
  dx_mat4_set_identity(&self->world_matrix);
  
  DX_OBJECT(self)->type = _type;
  return 0;
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
