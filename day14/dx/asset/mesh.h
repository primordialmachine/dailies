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
/// - "quadriliteral" a quadriliteral mesh
/// - "empty" an empty mesh
/// @return A pointer to the dx_asset_mesh object on success. A null pointer on failure.
dx_asset_mesh* dx_asset_mesh_create(char const* specifier);

/// @brief Pack the mesh data into a single stream of the specified format.
/// @param mesh A pointer to this mesh.
/// @param vertex_format The vertex format to pack the mesh data in.
/// @param bytes A pointer to a <code>void*</code> variable.
/// @param number_of_bytes A pointer to a <code>size_t</code> variable.
/// @return The zero value on success. A non-zero value on failure.
/// @success
/// <code>*number_of_bytes</code> was assigned the length, in Bytes, of the packed mesh data.
/// <code>*bytes</code> was assigned a pointer to an array of length <code>*number_of_bytes</code>.
int dx_asset_mesh_format(dx_asset_mesh* mesh, DX_VERTEX_FORMAT vertex_format, void **bytes, size_t *number_of_bytes);

/// @brief Append a quadriliteral.
/// @param mesh A pointer to the mesh.
/// @remarks
/// The width and height of each side is @a 1.
/// The center is <code>(0,0,0)</code>.
/// The normal is <code>(0,0,+1)</code>.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_mesh_append_quadriliteral(dx_asset_mesh* mesh);

/// @brief Remove all vertices.
/// @param mesh A pointer to the mesh.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_mesh_clear(dx_asset_mesh* mesh);

/// @brief Set the "mesh ambient "gba" value.
/// @param mesh A pointer to this mesh.
/// @param color A pointer to "mesh ambient rgba" value.
void dx_asset_mesh_set_mesh_ambient_rgba(dx_asset_mesh* mesh, DX_VEC4 const* value);

#endif // DX_ASSET_MESH_H_INCLUDED
