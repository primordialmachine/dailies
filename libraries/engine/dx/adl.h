/// @file dx/adl.h
/// @brief Invocation of the Asset Definition Language ("ADL") compiler.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)

#if !defined(DX_ADL_H_INCLUDED)
#define DX_ADL_H_INCLUDED

#include "dx/ddl.h"
#include "dx/asset/scene.h"

dx_asset_scene* dx_adl_compile(dx_ddl_node* node);

#endif // DX_ADL_H_INCLUDED
