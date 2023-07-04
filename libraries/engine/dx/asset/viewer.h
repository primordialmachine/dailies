/// @file dx/asset/viewer.h
/// @brief Representation of a "viewer" asset.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
#if !defined(DX_ASSET_VIEWER_H_INCLUDED)
#define DX_ASSET_VIEWER_H_INCLUDED

#include "dx/core.h"
typedef struct dx_asset_optics dx_asset_optics;

DX_DECLARE_OBJECT_TYPE("dx.asset.viewer",
                       dx_asset_viewer,
                       dx_object)

static inline dx_asset_viewer* DX_ASSET_VIEWER(void* p) {
  return (dx_asset_viewer*)p;
}

struct dx_asset_viewer {
  dx_object _parent;
  /// @brief A pointer to the optics of this viewer.
  /// @default dx_asset_optics_perspective with default values.
  dx_asset_optics* optics;
  /// @brief A pointer to the ADL name of this viewer.
  dx_string* name;
  DX_VEC3 source;
  DX_VEC3 up;
  DX_VEC3 target;
};

/// @brief Construct this viewer with default values.
/// @param self A pointer to this viewer object.
/// @param name A pointer to the ADL name of this viewer object.
/// @return The zero value on success. A non-zero value on failure.
/// @remarks The default values are up <code>(0,1,0)</code>, source <code>(0,0,0)</code>, and target <code>(0,0,-1)</code>.
/// @default-failure
int dx_asset_viewer_construct(dx_asset_viewer* self, dx_string* name);

dx_asset_viewer* dx_asset_viewer_create(dx_string* name);

#endif // DX_ASSET_VIEWER_H_INCLUDED
