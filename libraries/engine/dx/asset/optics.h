#if !defined(DX_ASSET_OPTICS_H_INCLUDED)
#define DX_ASSET_OPTICS_H_INCLUDED

#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.optics",
                       dx_asset_optics,
                       dx_object)

static inline dx_asset_optics* DX_ASSET_OPTICS(void* p) {
  return (dx_asset_optics*)p;
}

struct dx_asset_optics {
  dx_object _parent;
};

int dx_asset_optics_construct(dx_asset_optics* self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.optics_orthographic",
                       dx_asset_optics_orthographic,
                       dx_asset_optics)

static inline dx_asset_optics_orthographic* DX_ASSET_OPTICS_ORTHOGRAPHIC(void* p) {
  return (dx_asset_optics_orthographic*)p;
}

struct dx_asset_optics_orthographic {
  dx_asset_optics _parent;
  /// @brief The default values are 0.1.
  dx_f32 near;
  /// @brief The default value is 1000.0.
  dx_f32 far;
};

int dx_asset_optics_orthographic_construct(dx_asset_optics_orthographic* self);

dx_asset_optics_orthographic* dx_asset_optics_orthographic_create();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.optics_perspective",
                       dx_asset_optics_perspective,
                       dx_asset_optics)

static inline dx_asset_optics_perspective* DX_ASSET_OPTICS_PERSPECTIVE(void* p) {
  return (dx_asset_optics_perspective*)p;
}

struct dx_asset_optics_perspective {
  dx_asset_optics _parent;
  /// @brief The aspect ratio (the quotient of the width and the height of near plane).
  /// The default is 4:3 = 1.33... .
  /// If not provided, it is computed from the actual width and the actual height of the viewport.
  dx_f32* aspect_ratio;
  /// @brief The field of yiew angle in degrees.
  /// The angle between
  /// - a plane through the origin and the bottom of the near plane
  /// - a plane through the origin and the top of the near plane.
  /// The default is 60.0.
  dx_f32 field_of_view_y;
  /// @brief The default value is 0.1.
  dx_f32 near;
  /// @brief The default value is 1000.0.
  dx_f32 far;
};

int dx_asset_optics_perspective_construct(dx_asset_optics_perspective* self);

dx_asset_optics_perspective* dx_asset_optics_perspective_create();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ASSET_OPTICS_H_INCLUDED
