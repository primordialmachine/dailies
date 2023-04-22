#if !defined(DX_ASSET_MESH_GENERATORS_H_INCLUDED)
#define DX_ASSET_MESH_GENERATORS_H_INCLUDED

#include "dx/asset/mesh.h"

/// @brief Create an empty mesh.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_mesh_on_empty(dx_asset_mesh* mesh);

#endif // DX_ASSET_MESH_GENERATORS_H_INCLUDED
