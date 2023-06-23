#include "dx/asset/mesh_operation.h"

DX_DEFINE_ENUMERATION_TYPE("dx.asset.mesh_operation_kind", dx_asset_mesh_operation_kind)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.mesh_operation", dx_asset_mesh_operation, dx_object)

int dx_asset_mesh_operation_construct(dx_asset_mesh_operation* self, dx_asset_mesh_operation_kind kind) {
  dx_rti_type* type = dx_asset_mesh_operation_get_type();
  if (!type) {
    return 1;
  }
  self->kind = kind;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_mesh_operation_destruct;
  return 0;
}

void dx_asset_mesh_operation_destruct(dx_asset_mesh_operation* self)
{ }

int dx_asset_mesh_operation_apply(dx_asset_mesh_operation* self, dx_asset_mesh* mesh) {
  return self->apply(self, mesh);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
