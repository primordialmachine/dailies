#if !defined(DX_ASSET_VIEWER_INSTANCE_H_INCLUDED)
#define DX_ASSET_VIEWER_INSTANCE_H_INCLUDED

#include "dx/asset/viewer.h"
#include "dx/asset/reference.h"

/// @brief A viewer instance asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.viewer_instance",
                       dx_asset_viewer_instance,
                       dx_object)

static inline dx_asset_viewer_instance* DX_ASSET_VIEWER_INSTANCE(void* p) {
    return (dx_asset_viewer_instance*)p;
}

struct dx_asset_viewer_instance {
  dx_object _parent;
  /// @brief Reference to the viewer of this viewer instance.
  dx_asset_reference* viewer_reference;
  /// @brief The world matrix of this viewer instance.
  DX_MAT4 world_matrix;
};

/// @brief Construct this dx_asset_viewer_instance object.
/// @param self A pointer to this dx_asset_viewer_instance object.
/// @param viewer A pointer to the viewer of this viewer instance.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a viewer is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int dx_asset_viewer_instance_construct(dx_asset_viewer_instance* self, dx_asset_viewer* viewer);

/// @brief Create a dx_asset_viewer_instance object.
/// @param viewer A pointer to the viewer of this viewer instance.
/// @return A pointer to the dx_asset_viewer_instance object on success. The null pointer failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a viewer is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
dx_asset_viewer_instance* dx_asset_viewer_instance_create(dx_asset_viewer* viewer);

#endif // DX_ASSET_VIEWER_INSTANCE_H_INCLUDED
