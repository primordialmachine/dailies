#include "dx/asset/brushes.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void dx_asset_brush_destruct(dx_asset_brush* self)
{ }

DX_DEFINE_OBJECT_TYPE("dx.asset.brush", dx_asset_brush, dx_object)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void dx_asset_solid_brush_destruct(dx_asset_solid_brush* self)
{ }

DX_DEFINE_OBJECT_TYPE("dx.asset.solid_brush", dx_asset_solid_brush, dx_object)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void dx_asset_checkerboard_brush_destruct(dx_asset_checkerboard_brush* self)
{ }

DX_DEFINE_OBJECT_TYPE("dx.asset.checkerboard_brush", dx_asset_checkerboard_brush, dx_object)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
