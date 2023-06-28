#if !defined(DX_ASSET_MESH_INSTANCE_H_INCLUDED)
#define DX_ASSET_MESH_INSTANCE_H_INCLUDED

#include "dx/asset/mesh.h"

/// @brief A mesh instance asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.mesh_instance", dx_asset_mesh_instance, dx_object)

static inline dx_asset_mesh_instance* DX_ASSET_MESH_INSTANCE(void* p) {
    return (dx_asset_mesh_instance*)p;
}

struct dx_asset_mesh_instance {
  dx_object _parent;
  /// @brief The mesh of this mesh instance.
  dx_asset_mesh* mesh;
  /// @brief The world matrix of this mesh instance.
  DX_MAT4 world_matrix;
};

/// @brief Construct this dx_asset_mesh_instance object.
/// @param self A pointer to this dx_asset_mesh_instance object.
/// @param mesh A pointer to the mesh of this mesh instance.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a mesh is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int dx_asset_mesh_instance_construct(dx_asset_mesh_instance* self, dx_asset_mesh* mesh);

/// @brief Create a dx_asset_mesh_instance object.
/// @param mesh A pointer to the mesh of this mesh instance.
/// @return A pointer to the dx_asset_mesh_instance object on success. The null pointer failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a mesh is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
dx_asset_mesh_instance* dx_asset_mesh_instance_create(dx_asset_mesh* mesh);

#endif // DX_ASSET_MESH_INSTANCE_H_INCLUDED
