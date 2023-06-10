#if !defined(DX_SCENES_CREATE_ASSETS_H_INCLUDED)
#define DX_SCENES_CREATE_ASSETS_H_INCLUDED

#include "dx/asset/mesh_instance.h"

/// @brief Create a mesh instance asset.
/// @param adl_text A pointer to the ADL program text describing the mesh instance asset.
/// @param adl_text_length The length of the program text pointed to by @a adl_text.
/// @return A pointer to the mesh asset on success. The null pointer on failure.
dx_asset_mesh_instance* _create_mesh_instance_from_text(char const* adl_text, size_t adl_text_length);

#endif // DX_SCENES_CREATE_ASSETS_H_INCLUDED
