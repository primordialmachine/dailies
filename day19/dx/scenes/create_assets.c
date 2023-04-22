#include "dx/scenes/create_assets.h"

#include "dx/asset/palette.h"
#include "dx/adl/reader.h"
#include <string.h>

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

dx_adl_node* _parse_brush_descriptor_1() {
  dx_adl_parser* parser = dx_adl_parser_create();
  if (!parser) {
    return NULL;
  }
  static const char* INPUT =
    "{\n"
    "  numberOfCheckers : {\n"
    "    horizontal: 16,\n"
    "    vertical: 16,\n"
    "  },\n"
    "  checkerSize: {\n"
    "    horizontal: 128,"
    "    vertical: 128,"
    "  },\n"
    "  checkerColors: {\n"
    "    first: 'malachite',"
    "    second: 'amber',"
    "  },\n"
    "}\n"
    ;
  if (dx_adl_parser_set(parser, INPUT, strlen(INPUT))) {
    DX_UNREFERENCE(parser);
    parser = NULL;
    return NULL;
  }
  dx_adl_node* node = dx_adl_parser_run(parser);
  DX_UNREFERENCE(parser);
  parser = NULL;
  return node;
}

int _parse_brush_descriptor_2(DX_ASSET_CHECKERBOARD_BRUSH* brush, dx_asset_palette* palette) {
  dx_adl_node* root_node = NULL;
  dx_adl_node* child_node = NULL;
  dx_string* name1 = NULL;
  
  root_node = _parse_brush_descriptor_1();
  if (!root_node) {
    goto END;
  }

  {
    dx_adl_node* number_of_checkers_node = NULL;
    dx_adl_node* horizontal_node = NULL;
    dx_adl_node* vertical_node = NULL;
    //
    name1 = dx_string_create("numberOfCheckers", sizeof("numberOfCheckers") - 1);
    if (!name1) {
      goto END;
    }
    number_of_checkers_node = dx_adl_node_map_get(root_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!number_of_checkers_node) {
      goto END;
    }
    //
    name1 = dx_string_create("horizontal", sizeof("horizontal") - 1);
    if (!name1) {
      goto END;
    }
    horizontal_node = dx_adl_node_map_get(number_of_checkers_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!horizontal_node) {
      goto END;
    }
    if (dx_convert_utf8bytes_to_sz(horizontal_node->number->bytes, horizontal_node->number->number_of_bytes, &brush->number_of_checkers.horizontal)) {
      goto END;
    }
    //
    name1 = dx_string_create("vertical", sizeof("vertical") - 1);
    if (!name1) {
      goto END;
    }
    vertical_node = dx_adl_node_map_get(number_of_checkers_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!vertical_node) {
      goto END;
    }
    if (dx_convert_utf8bytes_to_sz(vertical_node->number->bytes, vertical_node->number->number_of_bytes, &brush->number_of_checkers.vertical)) {
      goto END;
    }
  }
  {
    dx_adl_node* checker_size_node = NULL;
    dx_adl_node* horizontal_node = NULL;
    dx_adl_node* vertical_node = NULL;
    //
    name1 = dx_string_create("checkerSize", sizeof("checkerSize") - 1);
    if (!name1) {
      goto END;
    }
    checker_size_node = dx_adl_node_map_get(root_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!checker_size_node) {
      goto END;
    }
    //
    name1 = dx_string_create("horizontal", sizeof("horizontal") - 1);
    if (!name1) {
      goto END;
    }
    horizontal_node = dx_adl_node_map_get(checker_size_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!horizontal_node) {
      goto END;
    }
    if (dx_convert_utf8bytes_to_sz(horizontal_node->number->bytes, horizontal_node->number->number_of_bytes, &brush->checker_size.horizontal)) {
      goto END;
    }
    //
    name1 = dx_string_create("vertical", sizeof("vertical") - 1);
    if (!name1) {
      goto END;
    }
    vertical_node = dx_adl_node_map_get(checker_size_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!vertical_node) {
      goto END;
    }
    if (dx_convert_utf8bytes_to_sz(vertical_node->number->bytes, vertical_node->number->number_of_bytes, &brush->checker_size.vertical)) {
      goto END;
    }
  }
  {
    dx_adl_node* checker_colors_node = NULL;
    dx_adl_node* node = NULL;
    dx_asset_palette_entry* entry;
    //
    name1 = dx_string_create("checkerColors", sizeof("checkerColors") - 1);
    if (!name1) {
      goto END;
    }
    checker_colors_node = dx_adl_node_map_get(root_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!checker_colors_node) {
      goto END;
    }
    //
    name1 = dx_string_create("first", sizeof("first") - 1);
    if (!name1) {
      goto END;
    }
    node = dx_adl_node_map_get(checker_colors_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!node) {
      goto END;
    }
    entry = dx_asset_palette_get(palette, node->string);
    if (!entry) {
      goto END;
    }
    brush->checker_colors.first = entry->value;
    //
    name1 = dx_string_create("second", sizeof("second") - 1);
    if (!name1) {
      goto END;
    }
    node = dx_adl_node_map_get(checker_colors_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!node) {
      goto END;
    }
    entry = dx_asset_palette_get(palette, node->string);
    if (!entry) {
      goto END;
    }
    brush->checker_colors.second = entry->value;
  }

END:
  if (name1) {
    DX_UNREFERENCE(name1);
    name1 = NULL;
  }
  if (root_node) {
    DX_UNREFERENCE(root_node);
    root_node = NULL;
  }
  return 0;
}

dx_asset_texture* _create_checkerbord_texture() {
  // Create a checkerboard texture.
  dx_asset_palette* palette = _create_defaul_palette();
  if (!palette) {
    return NULL;
  }
  DX_ASSET_CHECKERBOARD_BRUSH brush;
  memset(&brush, 0, sizeof(brush));
  brush._parent.flags = DX_ASSET_BRUSH_FLAGS_CHECKBERBOARD;
  if (_parse_brush_descriptor_2(&brush, palette)) {
    DX_UNREFERENCE(palette);
    palette = NULL;
    return NULL;
  }
  DX_UNREFERENCE(palette);
  palette = NULL;
  struct {
    size_t horizontal;
    size_t vertical;
  } number_of_pixels = {
    .horizontal = brush.number_of_checkers.horizontal * brush.checker_size.horizontal,
    .vertical = brush.number_of_checkers.vertical * brush.checker_size.vertical,
  };
  dx_asset_image* asset_image = dx_asset_image_create_rgb_u8(DX_PIXEL_FORMAT_RGB_U8, number_of_pixels.horizontal, number_of_pixels.vertical, &dx_colors_malachite);
  if (!asset_image) {
    return NULL;
  }
  if (dx_asset_image_fill(asset_image, 0, 0, asset_image->width, asset_image->height, (DX_ASSET_BRUSH const*)&brush)) {
    DX_UNREFERENCE(asset_image);
    asset_image = NULL;
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
