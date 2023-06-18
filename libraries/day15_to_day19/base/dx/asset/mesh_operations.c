#include "dx/asset/mesh_operations.h"

DX_DEFINE_ENUMERATION_TYPE("dx.asset.mesh_operation_kind", dx_asset_mesh_operation_kind)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.mesh_operation", dx_asset_mesh_operation, dx_object)

int dx_asset_mesh_operation_construct(dx_asset_mesh_operation* self, dx_asset_mesh_operation_kind kind) {
  self->kind = kind;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_mesh_operation_destruct;
  return 0;
}

void dx_asset_mesh_operation_destruct(dx_asset_mesh_operation* self)
{ }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_asset_mesh_operation_set_vertex_colors_construct(dx_asset_mesh_operation_set_vertex_colors* self) {
  if (dx_asset_mesh_operation_construct(DX_ASSET_MESH_OPERATION(self), dx_asset_mesh_operation_kind_set_vertex_colors)) {
    return 1;
  }
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_mesh_operation_set_vertex_colors_destruct;
  return 0;
}

void dx_asset_mesh_operation_set_vertex_colors_destruct(dx_asset_mesh_operation_set_vertex_colors* self) {
  dx_asset_mesh_operation_destruct(DX_ASSET_MESH_OPERATION(self));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
