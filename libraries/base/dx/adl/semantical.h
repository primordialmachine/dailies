#if !defined(DX_ADL_SEMANTICAL_H_INCLUDED)
#define DX_ADL_SEMANTICAL_H_INCLUDED

#include "dx/adl/syntactical.h"
#include "dx/asset/image.h"
#include "dx/asset/material.h"
#include "dx/asset/mesh.h"
#include "dx/asset/mesh_instance.h"
#include "dx/asset/palette.h"
#include "dx/asset/scene.h"
#include "dx/asset/texture.h"
#include "dx/adl/semantical/state.h"
#include "dx/adl/semantical/names.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_asset_scene* dx_adl_parse_scene(dx_adl_node* node, dx_adl_semantical_state* state);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#endif // DX_ADL_SEMANTICAL_H_INCLUDED
