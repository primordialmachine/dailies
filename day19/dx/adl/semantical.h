#if !defined(DX_ADL_SEMANTICAL_H_INCLUDED)
#define DX_ADL_SEMANTICAL_H_INCLUDED

#include "dx/adl/syntactical.h"
#include "dx/asset/image.h"
#include "dx/asset/material.h"
#include "dx/asset/mesh.h"
#include "dx/asset/mesh_instance.h"
#include "dx/asset/palette.h"
#include "dx/asset/texture.h"

DX_ASSET_BRUSH* dx_adl_parse_brush(dx_adl_node* node, dx_asset_palette* palette);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_asset_image* dx_adl_parse_image(dx_adl_node* node, dx_asset_palette* palette);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_asset_texture* dx_adl_parse_texture(dx_adl_node* node, dx_asset_palette* palette);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_asset_material* dx_adl_parse_material(dx_adl_node* node, dx_asset_palette* palette);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_asset_mesh* dx_adl_parse_mesh(dx_adl_node* node, dx_asset_palette* palette);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_asset_mesh_instance* dx_adl_parse_mesh_instance(dx_adl_node* node, dx_asset_palette* palette);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ADL_SEMANTICAL_H_INCLUDED
