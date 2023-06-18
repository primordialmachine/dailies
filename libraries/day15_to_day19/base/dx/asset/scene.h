#if !defined(DX_ASSET_SCENE_H_INCLUDED)
#define DX_ASSET_SCENE_H_INCLUDED

#include "dx/core.h"

/// @brief A scene asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.scene", dx_asset_scene, dx_object)

static inline dx_asset_scene* DX_ASSET_SCENE(void* p) {
    return (dx_asset_scene*)p;
}

struct dx_asset_scene {
  dx_object _parent;
  /// @brief The list of mesh instances.
  dx_object_array mesh_instances;
};

/// @brief Construct this dx_asset_scene object with default values.
/// @param self A pointer to this dx_asset_scene object.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int dx_asset_scene_construct(dx_asset_scene* self);

/// @brief Destruct this dx_asset_scene object.
/// @param self A pointer to this dx_asset_material object.
/// @undefined @a self does not point to a constructed dx_asset_material object.
void dx_asset_scene_destruct(dx_asset_scene* self);

/// @brief Create a dx_asset_scene object with default values.
/// @return A pointer to the dx_asset_material object on success. The null pointer failure.
/// @failure This function has set the error variable.
/// - #DX_ALLOCATION_FAILED an allocation failed
dx_asset_scene* dx_asset_scene_create();

#endif // DX_ASSET_SCENE_H_INCLUDED