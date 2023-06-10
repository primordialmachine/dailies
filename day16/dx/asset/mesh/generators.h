#if !defined(DX_ASSET_MESH_GENERATORS_H_INCLUDED)
#define DX_ASSET_MESH_GENERATORS_H_INCLUDED

#include "dx/asset/mesh.h"

/// @brief Create a triangle mesh.
/// @return
/// The zero value on success. A non-zero value on failure.
int dx_asset_mesh_on_triangle(dx_asset_mesh* mesh);

/// @brief Create an empty mesh.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_mesh_on_empty(dx_asset_mesh* mesh);

/// @brief Create a quadriliteral mesh.
/// @return The zero value on success. A non-zero value on failure.
/// @remarks
/// The width and height of each side is @a 1.
/// The center is <code>(0,0,0)</code>.
/// The normal is <code>(0,0,+1)</code>.
int dx_asset_mesh_on_quadriliteral(dx_asset_mesh* mesh);

#endif // DX_ASSET_MESH_GENERATORS_H_INCLUDED
