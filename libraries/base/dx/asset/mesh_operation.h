#if !defined(DX_ASSET_MESH_OPERATION_H_INCLUDED)
#define DX_ASSET_MESH_OPERATION_H_INCLUDED

#include "dx/asset/mesh.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("dx.asset.mesh_operation_kind",
                            dx_asset_mesh_operation_kind)

enum dx_asset_mesh_operation_kind {
  /// @brief Set the vertex colors of a mesh.
  dx_asset_mesh_operation_kind_set_vertex_colors,
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.mesh_operation",
                       dx_asset_mesh_operation,
                       dx_object)

static inline dx_asset_mesh_operation* DX_ASSET_MESH_OPERATION(void* p) {
  return (dx_asset_mesh_operation*)p;
}

struct dx_asset_mesh_operation {
  dx_object _parent;
  dx_asset_mesh_operation_kind kind;
  int (*apply)(dx_asset_mesh_operation* self, dx_asset_mesh* mesh);
};

int dx_asset_mesh_operation_construct(dx_asset_mesh_operation* self, dx_asset_mesh_operation_kind kind);

int dx_asset_mesh_operation_apply(dx_asset_mesh_operation* self, dx_asset_mesh* mesh);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ASSET_MESH_OPERATION_H_INCLUDED
