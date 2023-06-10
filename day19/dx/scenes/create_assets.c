#include "dx/scenes/create_assets.h"

#include "dx/asset/palette.h"
#include "dx/adl/syntactical.h"
#include "dx/adl/semantical.h"
// strlen
#include <string.h>
// malloc, free
#include <malloc.h>

static dx_asset_palette* _create_defaul_palette() {
  dx_asset_palette* palette = dx_asset_palette_create();
  if (!palette) {
    return NULL;
  }
#define DEFINE(NAME, R, G, B) \
  { \
    dx_string* name = dx_string_create(NAME, sizeof(NAME) - 1); \
    if (!name) { \
      DX_UNREFERENCE(palette); \
      palette = NULL; \
      return NULL; \
    } \
    DX_RGB_U8 value = { .r = R, .g = G, .b = B }; \
    if (dx_asset_palette_set(palette, name, &value)) { \
      DX_UNREFERENCE(name); \
      name = NULL; \
      DX_UNREFERENCE(palette); \
      palette = NULL; \
      return NULL; \
    } \
  }

  DEFINE("malachite", dx_colors_malachite.r, dx_colors_malachite.g, dx_colors_malachite.b);
  DEFINE("amber", dx_colors_amber.r, dx_colors_amber.g, dx_colors_amber.b);
  DEFINE("capri", dx_colors_capri.r, dx_colors_capri.g, dx_colors_capri.b);

#undef DEFINE

  return palette;
}


static dx_adl_parser* _create_parser() {
  dx_adl_diagnostics* diagnostics = dx_adl_diagnostics_create();
  if (!diagnostics) {
    return NULL;
  }
  dx_adl_scanner* scanner = dx_adl_scanner_create();
  if (!scanner) {
    DX_UNREFERENCE(diagnostics);
    diagnostics = NULL;
    return NULL;
  }
  dx_adl_parser* parser = dx_adl_parser_create(scanner, diagnostics);
  DX_UNREFERENCE(scanner);
  scanner = NULL;
  DX_UNREFERENCE(diagnostics);
  diagnostics = NULL;
  return parser;
}

dx_adl_node* _parse(char const* p, size_t n) {
  dx_adl_parser* parser = _create_parser();
  if (!parser) {
    return NULL;
  }
  if (dx_adl_parser_set(parser, p, n)) {
    DX_UNREFERENCE(parser);
    parser = NULL;
    return NULL;
  }
  dx_adl_node* node = dx_adl_parser_run(parser);
  DX_UNREFERENCE(parser);
  parser = NULL;
  return node;
}

dx_asset_mesh_instance* _create_mesh_instance_from_text(char const* adl_text, size_t adl_text_length) {
  dx_asset_palette* palette = _create_defaul_palette();
  if (!palette) {
    return NULL;
  }
  dx_adl_node* adl_node = _parse(adl_text, adl_text_length);
  if (!adl_node) {
    DX_UNREFERENCE(palette);
    palette = NULL;
    return NULL;
  }
  dx_asset_mesh_instance* asset_mesh_instance = dx_adl_parse_mesh_instance(adl_node, palette);
  DX_UNREFERENCE(adl_node);
  adl_node = NULL;
  DX_UNREFERENCE(palette);
  palette = NULL;
  return asset_mesh_instance;
}
