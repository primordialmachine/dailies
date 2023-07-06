#if !defined(DX_VAL_VIEWER_H_INCLUDED)
#define DX_VAL_VIEWER_H_INCLUDED

#include "dx/val/context.h"
#include "dx/asset/viewer_instance.h"

/// @brief The backend-representation of a viewer.
/// A dx.val.viewer usually represents a dx.asset.viewer_instance.
/// There was not use in introducing a dx.val.viewer_instance object (yet).
DX_DECLARE_OBJECT_TYPE("dx.val.viewer",
                       dx_val_viewer,
                       dx_object)

static inline dx_val_viewer* DX_VAL_VIEWER(void* p) {
  return (dx_val_viewer*)p;
}

struct dx_val_viewer {
  dx_object;
  /// @brief The asset viewer instance this viewer is based on.
  dx_asset_viewer_instance* asset_viewer_instance;
  /// @brief The view matrix of this viewer.
  DX_MAT4 view_matrix;
  /// @brief The projection matrix of this viewer.
  DX_MAT4 projection_matrix;
  /// @brief The source position of this viewer.
  DX_VEC3 source;
  /// @brief The target position of this viewer.
  DX_VEC3 target;
  /// @brief The up vector of this viewer.
  DX_VEC3 up;
};

int dx_val_viewer_construct(dx_val_viewer* self, dx_asset_viewer_instance* asset_viewer_instance);

dx_val_viewer* dx_val_viewer_create(dx_asset_viewer_instance* asset_viewer_instance);

int dx_val_viewer_get_projection_matrix(DX_MAT4* result, dx_val_viewer* self, dx_i32 canvas_width, dx_i32 canvas_height);

int dx_val_viewer_get_view_matrix(DX_MAT4* result, dx_val_viewer* self, dx_i32 canvas_width, dx_i32 canvas_height);

#endif // DX_VAL_VIEWER_H_INCLUDED
