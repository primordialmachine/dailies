#include "dx/asset/mesh_instance.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.mesh_instance",
                      dx_asset_mesh_instance,
                      dx_object)

int dx_asset_mesh_instance_construct(dx_asset_mesh_instance* self, dx_asset_reference* mesh_reference) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_asset_mesh_instance_get_type();
  if (!_type) {
    return 1;
  }

  if (!mesh_reference) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->mesh_reference = mesh_reference;
  DX_REFERENCE(self->mesh_reference);

  dx_mat4_set_identity(&self->world_matrix);

  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_asset_mesh_instance_destruct(dx_asset_mesh_instance* self) {
  DX_UNREFERENCE(self->mesh_reference);
  self->mesh_reference = NULL;
}

dx_asset_mesh_instance* dx_asset_mesh_instance_create(dx_asset_reference* mesh_reference) {
  dx_asset_mesh_instance* self = DX_ASSET_MESH_INSTANCE(dx_object_alloc(sizeof(dx_asset_mesh_instance)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_mesh_instance_construct(self, mesh_reference)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
