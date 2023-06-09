#if !defined(DX_ADL_SEMANTICAL_H_INCLUDED)
#define DX_ADL_SEMANTICAL_H_INCLUDED

#include "dx/adl/syntactical.h"
#include "dx/asset/image.h"
#include "dx/asset/palette.h"

DX_ASSET_BRUSH* dx_adl_parse_brush(dx_adl_node* node, dx_asset_palette* palette);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_asset_image* dx_adl_parse_image(dx_adl_node* node, dx_asset_palette* palette);

#endif // DX_ADL_SEMANTICAL_H_INCLUDED
