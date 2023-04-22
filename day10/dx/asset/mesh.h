#if !defined(DX_ASSET_MESH_H_INCLUDED)
#define DX_ASSET_MESH_H_INCLUDED

#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct dx_asset_mesh dx_asset_mesh;

static inline dx_asset_mesh* DX_ASSET_MESH(void* p) {
  return (dx_asset_mesh*)p;
}

struct dx_asset_mesh {
  dx_object _parent;

  uint32_t number_of_vertices;
 
  struct {
    /// The mesh ambient "rgba" value.
    DX_VEC4 ambient_rgba;
  } mesh;

  struct {
    /// Pointer to an array of number_of_vertices DX_VEC3 objects.
    /// These objects are the per-vertex "xyz" values.
    DX_VEC3* xyz;
    /// Pointer to an array of number_of_vertices DX_VEC4 objects.
    /// These objects are the per-vertex ambient "rgba" values.
    DX_VEC4* ambient_rgba;
  } vertices;
};

/// @brief Generate a mesh.
/// @param specifier the specifier
/// @remarks
/// A "specifier" specifies what mesh this function shall return.
/// The following specifiers are currently supported:
/// - "triangle" a triangle mesh
/// - "empty" an empty mesh
/// @return A pointer to the dx_asset_mesh object on success. A null pointer on failure.
dx_asset_mesh* dx_asset_mesh_create(char const* specifier);

/// @brief Remove all vertices.
/// @param mesh A pointer to the mesh.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_mesh_clear(dx_asset_mesh* mesh);

/// @brief Set the "mesh ambient "gba" value.
/// @param mesh A pointer to this mesh.
/// @param color A pointer to "mesh ambient rgba" value.
void dx_asset_mesh_set_mesh_ambient_rgba(dx_asset_mesh* mesh, DX_VEC4 const* value);

#endif // DX_ASSET_MESH_H_INCLUDED
