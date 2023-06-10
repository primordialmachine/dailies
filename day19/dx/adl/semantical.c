#include "dx/adl/semantical.h"

#include "dx/adl/syntactical.h"
#include "dx/asset/palette.h"
// malloc, free
#include <malloc.h>
// memset
#include <memory.h>
// strlen
#include <string.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_string* _parse_type(dx_adl_node* node);

static int _parse_n8(dx_adl_node* node, char const* name, dx_n8* target);
static int _parse_sz(dx_adl_node* node, char const* name, dx_size* target);

static dx_string* _parse_type(dx_adl_node* node) {
  dx_string* key = dx_string_create("type", sizeof("type") - 1);
  if (!key) {
    return NULL;
  }
  dx_adl_node* child_node = dx_adl_node_map_get(node, key);
  DX_UNREFERENCE(key);
  key = NULL;
  if (!child_node || child_node->type != dx_adl_node_type_string) {
    return NULL;
  }
  dx_string* type = dx_adl_node_get_string(child_node);
  DX_DEBUG_CHECK_MAGIC_BYTES(type);
  return type;
}

static int _parse_n8(dx_adl_node* node, char const* name, dx_n8* target) {
  dx_string* name1 = dx_string_create(name, strlen(name));
  if (!name) {
    return 1;
  }
  dx_adl_node* child_node = dx_adl_node_map_get(node, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!child_node || child_node->type != dx_adl_node_type_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_n8(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

static int _parse_sz(dx_adl_node* node, char const* name, dx_size* target) {
  dx_string* name1 = dx_string_create(name, strlen(name));
  if (!name) {
    return 1;
  }
  dx_adl_node* child_node = dx_adl_node_map_get(node, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!child_node || child_node->type != dx_adl_node_type_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_sz(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int _parse_solid_brush_1(DX_ASSET_SOLID_BRUSH* brush, dx_adl_node* root_node, dx_asset_palette* palette);

static int _parse_checkerboard_brush_1(DX_ASSET_CHECKERBOARD_BRUSH* brush, dx_adl_node* root_node, dx_asset_palette* palette);

static DX_ASSET_SOLID_BRUSH* _parse_solid_brush(dx_adl_node* node, dx_asset_palette* palette);

static DX_ASSET_CHECKERBOARD_BRUSH* _parse_checkerboard_brush(dx_adl_node* node, dx_asset_palette* palette);

static int _parse_solid_brush_1(DX_ASSET_SOLID_BRUSH* brush, dx_adl_node* root_node, dx_asset_palette* palette) {
  dx_string* name1 = NULL;
  if (!root_node) {
    goto END;
  }
  {
    dx_adl_node* node = NULL;
    dx_asset_palette_entry* entry;
    //
    name1 = dx_string_create("color", sizeof("color") - 1);
    if (!name1) {
      goto END;
    }
    node = dx_adl_node_map_get(root_node, name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!node || node->type != dx_adl_node_type_string) {
      goto END;
    }
    entry = dx_asset_palette_get(palette, node->string);
    if (!entry) {
      goto END;
    }
    brush->color = entry->value;
  }

END:
  if (name1) {
    DX_UNREFERENCE(name1);
    name1 = NULL;
  }
  return 0;
}

static int _parse_checkerboard_brush_1(DX_ASSET_CHECKERBOARD_BRUSH* brush, dx_adl_node* root_node, dx_asset_palette* palette) {
  dx_adl_node* child_node = NULL;
  dx_string* name1 = NULL;
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
    // numberOfCheckers.horizontal
    if (_parse_sz(number_of_checkers_node, "horizontal", &brush->number_of_checkers.horizontal)) {
      goto END;
    }
    // numberOfCheckers.vertical
    if (_parse_sz(number_of_checkers_node, "vertical", &brush->number_of_checkers.vertical)) {
      goto END;
    }
  }
  {
    dx_adl_node* checker_size_node = NULL;
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
    // checkerSize.horizontal
    if (_parse_sz(checker_size_node, "horizontal", &brush->checker_size.horizontal)) {
      goto END;
    }
    // checkerSize.vertical
    if (_parse_sz(checker_size_node, "vertical", &brush->checker_size.vertical)) {
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
  return 0;
}

static DX_ASSET_SOLID_BRUSH* _parse_solid_brush(dx_adl_node* node, dx_asset_palette* palette) {
  DX_ASSET_SOLID_BRUSH* brush = malloc(sizeof(DX_ASSET_SOLID_BRUSH));
  if (!brush) {
    return NULL;
  }
  memset(brush, 0, sizeof(DX_ASSET_SOLID_BRUSH));
  brush->_parent.flags = DX_ASSET_BRUSH_FLAGS_SOLID;
  if (_parse_solid_brush_1(brush, node, palette)) {
    free(brush);
    brush = NULL;
    return NULL;
  }
  return brush;
}

static DX_ASSET_CHECKERBOARD_BRUSH* _parse_checkerboard_brush(dx_adl_node* node, dx_asset_palette* palette) {
  DX_ASSET_CHECKERBOARD_BRUSH* brush = malloc(sizeof(DX_ASSET_CHECKERBOARD_BRUSH));
  if (!brush) {
    return NULL;
  }
  memset(brush, 0, sizeof(DX_ASSET_CHECKERBOARD_BRUSH));
  brush->_parent.flags = DX_ASSET_BRUSH_FLAGS_CHECKBERBOARD;
  if (_parse_checkerboard_brush_1(brush, node, palette)) {
    free(brush);
    brush = NULL;
    return NULL;
  }
  return brush;
}

DX_ASSET_BRUSH* dx_adl_parse_brush(dx_adl_node* node, dx_asset_palette* palette) {
  DX_ASSET_BRUSH* brush = NULL;
  dx_string* solid_type = NULL;
  dx_string* checkerboard_type = NULL;
  dx_string* type = NULL;
  
  type = _parse_type(node);
  if (!type) {
    return NULL;
  }
  solid_type = dx_string_create("SolidBrush", sizeof("SolidBrush") - 1);
  if (!solid_type) {
    goto END;
  }
  checkerboard_type = dx_string_create("CheckerboardBrush", sizeof("CheckerboardBrush") - 1);
  if (!checkerboard_type) {
    goto END;
  }

  if (dx_string_is_equal_to(type, solid_type)) {
    brush = (DX_ASSET_BRUSH*)_parse_solid_brush(node, palette);
    if (!brush) {
      goto END;
    }
  } else if (dx_string_is_equal_to(type, checkerboard_type)) {
    brush = (DX_ASSET_BRUSH*)_parse_checkerboard_brush(node, palette);
    if (!brush) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  if (checkerboard_type) {
    DX_UNREFERENCE(checkerboard_type);
    checkerboard_type = NULL;
  }
  if (solid_type) {
    DX_UNREFERENCE(solid_type);
    solid_type = NULL;
  }
  return brush;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_image* _parse_image(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_image* image = NULL;
  size_t width, height;
  DX_RGB_U8* color = NULL;
  DX_ASSET_BRUSH* brush = NULL;
  // width
  if (_parse_sz(node, "width", &width)) {
    goto END;
  }
  // height
  if (_parse_sz(node, "height", &height)) {
    goto END;
  }
  // color
  {
    dx_string* name = dx_string_create("color", sizeof("color") - 1);
    if (!name) {
      goto END;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      goto END;
    }
    dx_asset_palette_entry* palette_entry = dx_asset_palette_get(palette, child_node->string);
    if (!palette_entry) {
      goto END;
    }
    color = malloc(sizeof(DX_RGB_U8));
    if (!color) {
      goto END;
    }
    *color = palette_entry->value;
  }
  // brush
  {
    dx_string* name = dx_string_create("brush", sizeof("brush") - 1);
    if (!name) {
      goto END;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      goto END;
    }
    brush = dx_adl_parse_brush(child_node, palette);
    if (!brush) {
      goto END;
    }
  }
  image = dx_asset_image_create(DX_PIXEL_FORMAT_RGB_U8, width, height, color);
  if (!image) {
    goto END;
  }
  if (dx_asset_image_fill(image, 0, 0, image->width, image->height, brush)) {
    goto END;
  }
END:
  if (brush) {
    free(brush);
    brush = NULL;
  }
  if (color) {
    free(color);
    color = NULL;
  }
  return image;
}

dx_asset_image* dx_adl_parse_image(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_image* image = NULL;
  dx_string* image_type = NULL;
  dx_string* type = NULL;

  type = _parse_type(node);
  if (!type) {
    return NULL;
  }
  image_type = dx_string_create("Image", sizeof("Image") - 1);
  if (!image_type) {
    goto END;
  }
  if (dx_string_is_equal_to(type, image_type)) {
    image = _parse_image(node, palette);
    if (!image) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  if (image_type) {
    DX_UNREFERENCE(image_type);
    image_type = NULL;
  }
  return image;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_texture* _parse_texture(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_texture* texture = NULL;
  dx_asset_image* image = NULL;
  // image
  {
    dx_string* name = dx_string_create("image", sizeof("image") - 1);
    if (!name) {
      goto END;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      goto END;
    }
    image = dx_adl_parse_image(child_node, palette);
    if (!image) {
      goto END;
    }
  }
  texture = dx_asset_texture_create(image);
  DX_UNREFERENCE(image);
  image = NULL;
  if (!texture) {
    goto END;
  }
END:
  if (image) {
    DX_UNREFERENCE(image);
    image = NULL;
  }
  return texture;
}

dx_asset_texture* dx_adl_parse_texture(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_texture* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node);
  if (!received_type) {
    return NULL;
  }
  expected_type = dx_string_create("Texture", sizeof("Texture") - 1);
  if (!expected_type) {
    goto END;
  }
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_texture(node, palette);
    if (!asset) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  if (expected_type) {
    DX_UNREFERENCE(expected_type);
    expected_type = NULL;
  }
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_material* _parse_material(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_material* material = NULL;
  dx_asset_texture* ambient_texture = NULL;
  // ambientTexture?
  {
    dx_string* name = dx_string_create("ambientTexture", sizeof("ambientTexture") - 1);
    if (!name) {
      goto END;
    }
    dx_error old_error = dx_get_error();
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (child_node) {
      ambient_texture = dx_adl_parse_texture(child_node, palette);
      if (!ambient_texture) {
        goto END;
      }
    } else {
      dx_set_error(old_error);
    }
  }
  material = dx_asset_material_create();
  if (!material) {
    goto END;
  }
  if (dx_asset_material_set_ambient_texture(material, ambient_texture)) {
    goto END;
  }
END:
  if (ambient_texture) {
    DX_UNREFERENCE(ambient_texture);
    ambient_texture = NULL;
  }
  return material;
}

dx_asset_material* dx_adl_parse_material(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_material* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node);
  if (!received_type) {
    return NULL;
  }
  expected_type = dx_string_create("Material", sizeof("Material") - 1);
  if (!expected_type) {
    goto END;
  }
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_material(node, palette);
    if (!asset) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  if (expected_type) {
    DX_UNREFERENCE(expected_type);
    expected_type = NULL;
  }
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_mesh* _parse_mesh(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_mesh* mesh = NULL;
  dx_string* generator = NULL;
  dx_asset_material* material = NULL;
  // generator
  {
    dx_string* name = dx_string_create("generator", sizeof("generator") - 1);
    if (!name) {
      goto END;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      goto END;
    }
    generator = dx_adl_node_get_string(child_node);
    if (!generator) {
      goto END;
    }
  }
  // material
  {
    dx_string* name = dx_string_create("material", sizeof("material") - 1);
    if (!name) {
      goto END;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      goto END;
    }
    material = dx_adl_parse_material(child_node, palette);
    if (!material) {
      goto END;
    }
  }
  mesh = dx_asset_mesh_create(generator, material);
  if (!mesh) {
    goto END;
  }
END:
  if (material) {
    DX_UNREFERENCE(material);
    material = NULL;
  }
  return mesh;
}

dx_asset_mesh* dx_adl_parse_mesh(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_mesh* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node);
  if (!received_type) {
    return NULL;
  }
  expected_type = dx_string_create("Mesh", sizeof("Mesh") - 1);
  if (!expected_type) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_mesh(node, palette);
    if (!asset) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  if (expected_type) {
    DX_UNREFERENCE(expected_type);
    expected_type = NULL;
  }
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_mesh_instance* _parse_mesh_instance(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_mesh_instance* mesh_instance = NULL;
  dx_asset_mesh* mesh = NULL;
  // mesh
  {
    dx_string* name = dx_string_create("mesh", sizeof("mesh") - 1);
    if (!name) {
      goto END;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      goto END;
    }
    mesh = _parse_mesh(child_node, palette);
    if (!mesh) {
      goto END;
    }
  }
  mesh_instance = dx_asset_mesh_instance_create(mesh);
  if (!mesh_instance) {
    goto END;
  }
END:
  if (mesh) {
    DX_UNREFERENCE(mesh);
    mesh = NULL;
  }
  return mesh_instance;
}

dx_asset_mesh_instance* dx_adl_parse_mesh_instance(dx_adl_node* node, dx_asset_palette* palette) {
  dx_asset_mesh_instance* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node);
  if (!received_type) {
    return NULL;
  }
  expected_type = dx_string_create("MeshInstance", sizeof("MeshInstance") - 1);
  if (!expected_type) {
    goto END;
  }
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_mesh_instance(node, palette);
    if (!asset) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  if (expected_type) {
    DX_UNREFERENCE(expected_type);
    expected_type = NULL;
  }
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
