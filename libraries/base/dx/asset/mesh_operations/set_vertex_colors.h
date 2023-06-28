#if !defined(DX_ASSET_MESH_OPERATIONS_SET_VERTEX_COLORS_H_INCLUDED)
#define DX_ASSET_MESH_OPERATIONS_SET_VERTEX_COLORS_H_INCLUDED

#include "dx/asset/mesh_operation.h"

DX_DECLARE_OBJECT_TYPE("dx.asset.mesh_operations.set_vertex_colors",
                       dx_asset_mesh_operations_set_vertex_colors,
                       dx_object)

static inline dx_asset_mesh_operations_set_vertex_colors* DX_ASSET_MESH_OPERATIONS_SET_VERTEX_COLORS(void* p) {
  return (dx_asset_mesh_operations_set_vertex_colors*)p;
}

struct dx_asset_mesh_operations_set_vertex_colors {
  dx_asset_mesh_operation _parent;
};

int dx_asset_mesh_operations_set_vertex_colors_construct(dx_asset_mesh_operations_set_vertex_colors* self);

dx_asset_mesh_operations_set_vertex_colors* dx_asset_mesh_operations_set_vertex_colors_create();

#endif // DX_ASSET_MESH_OPERATIONS_SET_VERTEX_COLORS_H_INCLUDED
