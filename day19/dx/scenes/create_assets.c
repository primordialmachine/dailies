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

static char const* SOLID_IMAGE_DESCRIPTOR = 
  "{\n"
  "  type : 'Image',\n"
  "  width : 2048,\n"
  "  height : 2048,\n"
  "  color : 'malachite',\n"
  "  brush :{\n"
  "    type : 'SolidBrush',\n"
  "    color : 'amber',\n"
  "  },\n"
  "}\n"
  ;

static char const* CHECKERBOARD_IMAGE_DESCRIPTOR =
  "{\n"
  "  type : 'Image',\n"
  "  width : 2048,\n"
  "  height : 2048,\n"
  "  color : 'malachite',\n"
  "  brush :{\n"
  "    type : 'CheckerboardBrush',\n"
  "    numberOfCheckers : {\n"
  "      horizontal: 16,\n"
  "      vertical: 16,\n"
  "    },\n"
  "    checkerSize: {\n"
  "      horizontal: 128,\n"
  "      vertical: 128,\n"
  "    },\n"
  "    checkerColors: {\n"
  "      first: 'malachite',\n"
  "      second: 'amber',\n"
  "    },\n"
  "  },\n"
  "}\n"
  ;

dx_adl_node* _parse(char const* p, size_t n) {
  dx_adl_parser* parser = dx_adl_parser_create();
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

dx_asset_texture* _create_checkerbord_texture() {
  dx_asset_palette* palette = _create_defaul_palette();
  if (!palette) {
    return NULL;
  }
  //dx_adl_node* image_node = _parse(SOLID_IMAGE_DESCRIPTOR, strlen(SOLID_IMAGE_DESCRIPTOR));
  dx_adl_node* image_node = _parse(CHECKERBOARD_IMAGE_DESCRIPTOR, strlen(CHECKERBOARD_IMAGE_DESCRIPTOR));
  if (!image_node) {
    DX_UNREFERENCE(palette);
    palette = NULL;
    return NULL;
  }
  dx_asset_image* asset_image = dx_adl_parse_image(image_node, palette);
  if (!asset_image) {
    DX_UNREFERENCE(palette);
    palette = NULL;
    return NULL;
  }
  dx_asset_texture* asset_texture = dx_asset_texture_create(asset_image);
  DX_UNREFERENCE(asset_image);
  asset_image = NULL;
  return asset_texture;
}

dx_asset_material* _create_checkerboard_material() {
  dx_asset_material* material = dx_asset_material_create();
  if (!material) {
    return NULL;
  }
  dx_asset_texture* texture = _create_checkerbord_texture();
  if (!texture) {
    DX_UNREFERENCE(material);
    material = NULL;
    return NULL;
  }
  if (dx_asset_material_set_ambient_texture(material, texture)) {
    DX_UNREFERENCE(material);
    material = NULL;
    DX_UNREFERENCE(texture);
    texture = NULL;
    return NULL;
  }
  DX_UNREFERENCE(texture);
  texture = NULL;
  return material;
}

dx_asset_mesh* _create_mesh(char const* specifier, dx_asset_material* material) {
  dx_asset_mesh* mesh = dx_asset_mesh_create(specifier, material);
  if (!mesh) {
    return NULL;
  }
  return mesh;
}
