#include "dx/scenes/create_assets.h"

#include "dx/adl.h"

dx_asset_scene* _create_scene_from_text(char const* adl_text, dx_size adl_text_length) {
  dx_ddl_node* ddl_node = dx_ddl_compile(adl_text, adl_text_length);
  if (!ddl_node) {
    return NULL;
  }
  dx_asset_scene* asset_scene = dx_adl_compile(ddl_node);
  DX_UNREFERENCE(ddl_node);
  ddl_node = NULL;
  return asset_scene;
}
