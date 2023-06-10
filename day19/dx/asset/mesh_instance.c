#include "dx/asset/mesh_instance.h"

int dx_asset_mesh_instance_construct(dx_asset_mesh_instance* self, dx_asset_mesh* mesh) {
  if (!self || !mesh) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->mesh = mesh;
  DX_REFERENCE(self->mesh);
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_asset_mesh_instance_destruct;
  return 0;
}

void dx_asset_mesh_instance_destruct(dx_asset_mesh_instance* self) {
  DX_UNREFERENCE(self->mesh);
  self->mesh = NULL;
}

dx_asset_mesh_instance* dx_asset_mesh_instance_create(dx_asset_mesh* mesh) {
  dx_asset_mesh_instance* self = DX_ASSET_MESH_INSTANCE(dx_object_alloc(sizeof(dx_asset_mesh_instance)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_mesh_instance_construct(self, mesh)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
