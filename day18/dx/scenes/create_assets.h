#if !defined(DX_SCENES_CREATE_ASSETS_H_INCLUDED)
#define DX_SCENES_CREATE_ASSETS_H_INCLUDED

#include "dx/asset/image.h"
#include "dx/asset/material.h"
#include "dx/asset/mesh.h"
#include "dx/asset/texture.h"

/// @brief Create a checkboard texture asset.
/// @return A pointer to the checkerboard texture asset on success. The null pointer on failure.
dx_asset_texture* _create_checkerbord_texture();

/// @brief Create a checkerboar material asset.
/// @return A pointer to the checkerboard material asset on success. The null pointer on failure.
dx_asset_material* _create_checkerboard_material();

/// @brief Create a mesh asset.
/// @param specifier The specifier of the mesh asset.
/// @param material The material asset of the mesh asset.
/// @return A pointer to the mesh asset on success. A null pointer on failure.
dx_asset_mesh* _create_mesh(char const* specifier, dx_asset_material* material);

#endif // DX_SCENES_CREATE_ASSETS_H_INCLUDED
