#include "dx/scenes/create_assets.h"

#include "dx/asset/palette.h"
#include "dx/adl/syntactical.h"
#include "dx/adl/semantical.h"
// strlen
#include <string.h>
// malloc, free
#include <malloc.h>

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

dx_asset_scene* _create_scene_from_text(char const* adl_text, size_t adl_text_length) {
  dx_adl_semantical_state* state = dx_adl_semantical_state_create();
  if (!state) {
    return NULL;
  }
  dx_adl_semantical_names* names = dx_adl_semantical_names_create();
  if (!names) {
    DX_UNREFERENCE(state);
    state = NULL;
    return NULL;
  }
  dx_adl_node* adl_node = _parse(adl_text, adl_text_length);
  if (!adl_node) {
    DX_UNREFERENCE(names);
    names = NULL;
    DX_UNREFERENCE(state);
    state = NULL;
    return NULL;
  }
  dx_asset_scene* asset_scene = dx_adl_parse_scene(adl_node, state, names);
  DX_UNREFERENCE(adl_node);
  adl_node = NULL;
  DX_UNREFERENCE(names);
  names = NULL;
  DX_UNREFERENCE(state);
  state = NULL;
  return asset_scene;
}
