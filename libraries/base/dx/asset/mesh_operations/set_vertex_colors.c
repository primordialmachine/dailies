#include "dx/asset/mesh_operations/set_vertex_colors.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.mesh_operations.set_vertex_colors",
                      dx_asset_mesh_operations_set_vertex_colors,
                      dx_asset_mesh_operation)

static const DX_VEC4 colors[] = {
  { 1.f, 0.f, 0.f, 1.f },
  { 0.f, 1.f, 0.f, 1.f },
  { 0.f, 0.f, 1.f, 1.f },
  //
  { 1.f, 1.f, 0.f, 1.f },
  { 0.f, 1.f, 1.f, 1.f },
  { 1.f, 0.f, 1.f, 1.f },
};

static DX_VEC4 const* get_color(dx_size index) {
  return &(colors[index % 6]);
}

static int apply(dx_asset_mesh_operations_set_vertex_colors* self, dx_asset_mesh* mesh) {
  for (dx_size i = 0, n = mesh->number_of_vertices; i < n; ++i) {
    mesh->vertices.ambient_rgba[i] = *get_color(i);
  }
  return 0;
}

int dx_asset_mesh_operations_set_vertex_colors_construct(dx_asset_mesh_operations_set_vertex_colors* self) {
  dx_rti_type* type = dx_asset_mesh_operations_set_vertex_colors_get_type();
  if (!type) {
    return 1;
  }
  if (dx_asset_mesh_operation_construct(DX_ASSET_MESH_OPERATION(self), dx_asset_mesh_operation_kind_set_vertex_colors)) {
    return 1;
  }
  DX_ASSET_MESH_OPERATION(self)->apply = (int(*)(dx_asset_mesh_operation*, dx_asset_mesh*)) & apply;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_asset_mesh_operations_set_vertex_colors_destruct;
  return 0;
}

void dx_asset_mesh_operations_set_vertex_colors_destruct(dx_asset_mesh_operations_set_vertex_colors* self) {
  dx_asset_mesh_operation_destruct(DX_ASSET_MESH_OPERATION(self));
}

dx_asset_mesh_operations_set_vertex_colors* dx_asset_mesh_operations_set_vertex_colors_create() {
  dx_rti_type* _type = dx_asset_mesh_operations_set_vertex_colors_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_mesh_operations_set_vertex_colors* self = DX_ASSET_MESH_OPERATIONS_SET_VERTEX_COLORS(dx_object_alloc(sizeof(dx_asset_mesh_operations_set_vertex_colors)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_mesh_operations_set_vertex_colors_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;  
}
