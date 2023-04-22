#if !defined(DX_MESH_H_INCLUDED)
#define DX_MESH_H_INCLUDED

#include "dx/asset/mesh.h"
#include "dx/asset/texture.h"
#include "dx/vbinding.h"
#include "dx/buffer.h"
#include "dx/program.h"
#include "dx/material.h"

#define DX_MESH_WITH_MATERIAL (1)

/// @brief The backend-representation of a mesh.
/// A dx.mesh usually represents a dx.asset.mesh.
typedef struct dx_mesh dx_mesh;
static inline dx_mesh* DX_MESH(void* p) {
  return (dx_mesh*)p;
}

struct dx_mesh {
  dx_object _parent;
  /// @brief A pointer to the corresponding asset mesh.
  dx_asset_mesh* asset_mesh;
  /// @brief A pointer to the corresponding assset material.
  /// Usually the asset material of the asset mesh.
  dx_material* material;
  dx_context* context;
  dx_buffer* buffer;
  dx_vbinding* vbinding;
  dx_program* program;
};

int dx_mesh_construct(dx_mesh* self, dx_context* context, dx_asset_mesh* asset_mesh);

void dx_mesh_destruct(dx_mesh* self);

dx_mesh* dx_mesh_create(dx_context* context, dx_asset_mesh* asset_mesh);

#endif // DX_MESH_H_INCLUDED
