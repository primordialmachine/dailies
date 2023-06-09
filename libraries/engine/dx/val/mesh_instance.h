#if !defined(DX_VAL_MESH_INSTANCE_H_INCLUDED)
#define DX_VAL_MESH_INSTANCE_H_INCLUDED

#include "dx/val/material.h"
#include "dx/val/mesh.h"

/// @brief The instance of a mesh in world space.
DX_DECLARE_OBJECT_TYPE("dx.val.mesh_instance",
                       dx_val_mesh_instance,
                       dx_object)

static dx_val_mesh_instance* DX_VAL_MESH_INSTANCE(void* p) {
  return (dx_val_mesh_instance*)p;
}

struct dx_val_mesh_instance {
  dx_object _parent;
  /// @brief The mesh of this mesh instance.
  dx_mesh* mesh;
  /// @brief The world matrix of this mesh instance.
  DX_MAT4 world_matrix;
  /// @brief The commands to render this mesh instance.
  dx_command_list* commands;
};

int dx_val_mesh_instance_construct(dx_val_mesh_instance* self, DX_MAT4 world_matrix, dx_mesh* mesh);

dx_val_mesh_instance* dx_val_mesh_instance_create(DX_MAT4 world_matrix, dx_mesh* mesh);

/// @brief Get the constant binding of this mesh instance.
/// @param self This mesh instance.
/// @return The constant binding of this mesh instance on success. The null pointer on failure.
dx_cbinding* dx_val_mesh_instance_get_cbinding(dx_val_mesh_instance* self);

/// @brief Update the specified constant binding with the constant binding of this mesh instance.
/// @param self This mesh instance.
/// @param cbinding The constant binding.
/// @return The zero value on success. A non-zero value on failure.
int dx_val_mesh_instance_update_cbinding(dx_val_mesh_instance* self, dx_cbinding* cbinding);

#endif // DX_VAL_MESH_INSTANCE_H_INCLUDED
