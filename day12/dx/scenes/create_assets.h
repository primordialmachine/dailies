#if !defined(DX_SCENES_CREATE_ASSETS_H_INCLUDED)
#define DX_SCENES_CREATE_ASSETS_H_INCLUDED

#include "dx/asset/mesh_instance.h"
#include "dx/asset/scene.h"

/// @brief Create a scene asset.
/// @param adl_text A pointer to the ADL program text describing the scene asset.
/// @param adl_text_length The length of the program text pointed to by @a adl_text.
/// @return A pointer to the scene asset on success. The null pointer on failure.
dx_asset_scene* _create_scene_from_text(char const* adl_text, dx_size adl_text_length);

#endif // DX_SCENES_CREATE_ASSETS_H_INCLUDED
