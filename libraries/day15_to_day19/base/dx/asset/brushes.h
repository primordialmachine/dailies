#if !defined(DX_ASSET_BRUSHES_H_INCLUDED)
#define DX_ASSET_BRUSHES_H_INCLUDED

#include "dx/asset/color.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.brush", dx_asset_brush, dx_object)

static inline dx_asset_brush* DX_ASSET_BRUSH(void* p) {
  return (dx_asset_brush*)p;
}

struct dx_asset_brush {
  dx_object _parent;
};

int dx_asset_brush_construct(dx_asset_brush* self);

void dx_asset_brush_destruct(dx_asset_brush* self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.solid_brush", dx_asset_solid_brush, dx_object)

static inline dx_asset_solid_brush* DX_ASSET_SOLID_BRUSH(void* p) {
  return (dx_asset_solid_brush*)p;
}

struct dx_asset_solid_brush {
  dx_asset_brush _parent;
  dx_asset_color* color;
  
};

int dx_asset_solid_brush_construct(dx_asset_solid_brush* self, dx_asset_color* color);

void dx_asset_solid_brush_destruct(dx_asset_solid_brush* self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.checkerboard_brush", dx_asset_checkerboard_brush, dx_object)

static inline dx_asset_checkerboard_brush* DX_ASSET_CHECKERBOARD_BRUSH(void* p) {
  return (dx_asset_checkerboard_brush*)p;
}

struct dx_asset_checkerboard_brush {
  dx_asset_brush _parent;
};

int dx_asset_checkerboard_brush_construct(dx_asset_checkerboard_brush* self);

void dx_asset_checkerboard_brush_destruct(dx_asset_checkerboard_brush* self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ASSET_BRUSHES_H_INCLUDED
