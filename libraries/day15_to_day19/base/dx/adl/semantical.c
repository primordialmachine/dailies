#include "dx/adl/semantical.h"

#include "dx/adl/syntactical.h"
#include "dx/asset/palette.h"
// malloc, free
#include <malloc.h>
// memset
#include <memory.h>
// strlen
#include <string.h>

static inline dx_string* _get_name(dx_adl_semantical_names* names, size_t index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}
#define NAME(name) _get_name((names), dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_string* _parse_type(dx_adl_node* node, dx_adl_semantical_names* names);

static int _parse_n8(dx_adl_node* node, dx_string* name, dx_n8* target);
static int _parse_sz(dx_adl_node* node, dx_string* name, dx_size* target);
static int _parse_f32(dx_adl_node* node, dx_string* name, dx_f32* target);
static int _parse_f64(dx_adl_node* node, dx_string* name, dx_f64* target);

/// @param palette A pointer to a palette or the null pointer.
static int _parse_rgb_u8(dx_adl_node* node, char const* name, dx_asset_palette* palette, DX_RGB_U8* target);

static dx_string* _parse_type(dx_adl_node* node, dx_adl_semantical_names* names) {
  dx_string* key = NAME(type_key);
  dx_adl_node* child_node = dx_adl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_adl_node_kind_string) {
    return NULL;
  }
  dx_string* type = dx_adl_node_get_string(child_node);
  DX_DEBUG_CHECK_MAGIC_BYTES(type);
  return type;
}

static int _parse_n8(dx_adl_node* node, dx_string* name, dx_n8* target) {
  dx_adl_node* child_node = dx_adl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_adl_node_kind_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_n8(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

static int _parse_sz(dx_adl_node* node, dx_string* name, dx_size* target) {
  dx_adl_node* child_node = dx_adl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_adl_node_kind_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_sz(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

static int _parse_f32(dx_adl_node* node, dx_string* name, dx_f32* target) {
  dx_adl_node* child_node = dx_adl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_adl_node_kind_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_f32(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

static int _parse_f64(dx_adl_node* node, dx_string* name, dx_f64* target) {
  dx_adl_node* child_node = dx_adl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_adl_node_kind_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_f64(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

static int _parse_rgb_u8(dx_adl_node* node, char const* name, dx_asset_palette* palette, DX_RGB_U8* target) {
  dx_string* name1 = dx_string_create(name, strlen(name));
  if (!name1) {
    return 1;
  }
  dx_adl_node* child_node = dx_adl_node_map_get(node, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!child_node || child_node->kind != dx_adl_node_kind_string) {
    return 1;
  }
  dx_asset_palette_entry* palette_entry = palette ? dx_asset_palette_get(palette, child_node->string) : NULL;
  if (!palette_entry) {
    return 1;
  }
  (*target) = palette_entry->value;
  return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int _parse_translation(DX_MAT4* target, dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static DX_MAT4* dx_adl_parse_translation(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static int _parse_solid_brush_1(DX_ASSET_SOLID_BRUSH* target, dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static int _parse_checkerboard_brush_1(DX_ASSET_CHECKERBOARD_BRUSH* target, dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static DX_ASSET_SOLID_BRUSH* _parse_solid_brush(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static DX_ASSET_CHECKERBOARD_BRUSH* _parse_checkerboard_brush(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

DX_ASSET_BRUSH* dx_adl_parse_brush(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_image* _parse_image(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_image* dx_adl_parse_image(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_texture* _parse_texture(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_texture* dx_adl_parse_texture(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_material* _parse_material(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_material* dx_adl_parse_material(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_mesh* _parse_mesh(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_mesh* dx_adl_parse_mesh(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_mesh_instance* _parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_mesh_instance* dx_adl_parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_scene* _parse_scene(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int _parse_translation(DX_MAT4* target, dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  if (!target || !node || !state) {
    return 1;
  }
  dx_f32 x, y, z;
  if (_parse_f32(node, NAME(x_key), &x)) {
    return 1;
  }
  if (_parse_f32(node, NAME(y_key), &y)) {
    return 1;
  }
  if (_parse_f32(node, NAME(z_key), &z)) {
    return 1;
  }
  dx_mat4_set_translate(target, x, y, z);
  return 0;
}

static DX_MAT4* dx_adl_parse_translation(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  DX_MAT4* target = malloc(sizeof(DX_MAT4));
  if (!target) {
    return NULL;
  }
  memset(target, 0, sizeof(DX_MAT4));
  if (_parse_translation(target, node, state, names)) {
    free(target);
    target = NULL;
    return NULL;
  }
  return target;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int _parse_solid_brush_1(DX_ASSET_SOLID_BRUSH* target, dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  if (!node) {
    goto END;
  }
  if (_parse_rgb_u8(node, "color", state->scene->palette, &target->color)) {
    goto END;
  }
END:
  return 0;
}

static int _parse_checkerboard_brush_1(DX_ASSET_CHECKERBOARD_BRUSH* target, dx_adl_node* root_node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
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
    if (_parse_sz(number_of_checkers_node, NAME(horizontal_key), &target->number_of_checkers.horizontal)) {
      goto END;
    }
    // numberOfCheckers.vertical
    if (_parse_sz(number_of_checkers_node, NAME(vertical_key), &target->number_of_checkers.vertical)) {
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
    if (_parse_sz(checker_size_node, NAME(horizontal_key), &target->checker_size.horizontal)) {
      goto END;
    }
    // checkerSize.vertical
    if (_parse_sz(checker_size_node, NAME(vertical_key), &target->checker_size.vertical)) {
      goto END;
    }
  }
  {
    dx_adl_node* checker_colors_node = NULL;
    dx_adl_node* node = NULL;
    dx_asset_palette_entry* entry = NULL;
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
    // checkerColors.first
    if (_parse_rgb_u8(checker_colors_node, "first", state->scene->palette, &target->checker_colors.first)) {
      goto END;
    }
    // checkerColors.second
    if (_parse_rgb_u8(checker_colors_node, "second", state->scene->palette, &target->checker_colors.second)) {
      goto END;
    }
  }
END:
  if (name1) {
    DX_UNREFERENCE(name1);
    name1 = NULL;
  }
  return 0;
}

static DX_ASSET_SOLID_BRUSH* _parse_solid_brush(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  DX_ASSET_SOLID_BRUSH* brush = malloc(sizeof(DX_ASSET_SOLID_BRUSH));
  if (!brush) {
    return NULL;
  }
  memset(brush, 0, sizeof(DX_ASSET_SOLID_BRUSH));
  brush->_parent.flags = DX_ASSET_BRUSH_FLAGS_SOLID;
  if (_parse_solid_brush_1(brush, node, state, names)) {
    free(brush);
    brush = NULL;
    return NULL;
  }
  return brush;
}

static DX_ASSET_CHECKERBOARD_BRUSH* _parse_checkerboard_brush(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  DX_ASSET_CHECKERBOARD_BRUSH* brush = malloc(sizeof(DX_ASSET_CHECKERBOARD_BRUSH));
  if (!brush) {
    return NULL;
  }
  memset(brush, 0, sizeof(DX_ASSET_CHECKERBOARD_BRUSH));
  brush->_parent.flags = DX_ASSET_BRUSH_FLAGS_CHECKBERBOARD;
  if (_parse_checkerboard_brush_1(brush, node, state, names)) {
    free(brush);
    brush = NULL;
    return NULL;
  }
  return brush;
}

static DX_ASSET_BRUSH* dx_adl_parse_brush(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  DX_ASSET_BRUSH* brush = NULL;
  dx_string* solid_type = NULL;
  dx_string* checkerboard_type = NULL;
  dx_string* type = NULL;
  
  type = _parse_type(node, names);
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
    brush = (DX_ASSET_BRUSH*)_parse_solid_brush(node, state, names);
    if (!brush) {
      goto END;
    }
  } else if (dx_string_is_equal_to(type, checkerboard_type)) {
    brush = (DX_ASSET_BRUSH*)_parse_checkerboard_brush(node, state, names);
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

static dx_asset_image* _parse_image(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_image* image = NULL;
  size_t width, height;
  DX_RGB_U8* color = NULL;
  DX_ASSET_BRUSH* brush = NULL;
  // width
  if (_parse_sz(node, NAME(width_key), &width)) {
    goto END;
  }
  // height
  if (_parse_sz(node, NAME(height_key), &height)) {
    goto END;
  }
  // color
  {
    color = malloc(sizeof(DX_RGB_U8));
    if (!color) {
      goto END;
    }
    if (_parse_rgb_u8(node, "color", state->scene->palette, color)) {
      goto END;
    }
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
    brush = dx_adl_parse_brush(child_node, state, names);
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

static dx_asset_image* dx_adl_parse_image(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_image* image = NULL;
  dx_string* image_type = NULL;
  dx_string* type = NULL;

  type = _parse_type(node, names);
  if (!type) {
    return NULL;
  }
  image_type = NAME(image_type);
  if (dx_string_is_equal_to(type, image_type)) {
    image = _parse_image(node, state, names);
    if (!image) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  return image;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_texture* _parse_texture(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_texture* texture = NULL;
  dx_asset_image* image = NULL;
  // image
  {
    dx_string* name = NAME(image_key);
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (!child_node) {
      goto END;
    }
    image = dx_adl_parse_image(child_node, state, names);
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

static dx_asset_texture* dx_adl_parse_texture(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_texture* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  expected_type = NAME(texture_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_texture(node, state, names);
    if (!asset) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_material* _parse_material(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
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
      ambient_texture = dx_adl_parse_texture(child_node, state, names);
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

static dx_asset_material* dx_adl_parse_material(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_material* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  expected_type = NAME(material_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_material(node, state, names);
    if (!asset) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_mesh* _parse_mesh(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
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
    material = dx_adl_parse_material(child_node, state, names);
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

static dx_asset_mesh* dx_adl_parse_mesh(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_mesh* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  expected_type = NAME(mesh_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_mesh(node, state, names);
    if (!asset) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_mesh_instance* _parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_mesh_instance* mesh_instance = NULL;
  dx_asset_mesh* mesh = NULL;
  DX_MAT4* transformation = NULL;
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
    mesh = _parse_mesh(child_node, state, names);
    if (!mesh) {
      goto END;
    }
  }
  // transformation?
  {
    dx_string* name = dx_string_create("transformation", sizeof("transformation") - 1);
    if (!name) {
      goto END;
    }
    dx_error old_error = dx_get_error();
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      if (dx_get_error() == DX_NOT_FOUND) {
        dx_set_error(old_error);
      } else {
        goto END;
      }
    } else {
      transformation = dx_adl_parse_translation(child_node, state, names);
      if (!transformation) {
        goto END;
      }
    }
  }
  mesh_instance = dx_asset_mesh_instance_create(mesh);
  if (!mesh_instance) {
    goto END;
  }
  if (transformation) {
    mesh_instance->world_matrix = *transformation;
  }
END:
  if (mesh) {
    DX_UNREFERENCE(mesh);
    mesh = NULL;
  }
  if (transformation) {
    free(transformation);
    transformation = NULL;
  }
  return mesh_instance;
}

static dx_asset_mesh_instance* dx_adl_parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_mesh_instance* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  expected_type = dx_string_create("MeshInstance", sizeof("MeshInstance") - 1);
  if (!expected_type) {
    goto END;
  }
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_mesh_instance(node, state, names);
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

static dx_asset_scene* _parse_scene(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_scene* scene = dx_asset_scene_create();
  if (!scene) {
    goto END;
  }
  // !!! store the scene in the state !!!
  state->scene = scene;
  DX_REFERENCE(state->scene);
  // meshInstances
  {
    dx_string* name = dx_string_create("meshInstances", sizeof("meshInstances") - 1);
    if (!name) {
      DX_UNREFERENCE(scene);
      scene = NULL;
      goto END;
    }
    dx_adl_node* node1 = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!node1 || node1->kind != dx_adl_node_kind_list) {
      DX_UNREFERENCE(scene);
      scene = NULL;
      goto END;
    }
    for (size_t i = 0, n = dx_adl_node_list_get_size(node1); i < n; ++i) {
      dx_adl_node* node2 = dx_adl_node_list_get(node1, i);
      if (!node2) {
        DX_UNREFERENCE(scene);
        scene = NULL;
        goto END;
      }
      dx_asset_mesh_instance* mesh_instance = _parse_mesh_instance(node2, state, names);
      if (!mesh_instance) {
        DX_UNREFERENCE(scene);
        scene = NULL;
        goto END;
      }
      if (dx_object_array_append(&scene->mesh_instances, DX_OBJECT(mesh_instance))) {
        DX_UNREFERENCE(mesh_instance);
        mesh_instance = NULL;
        DX_UNREFERENCE(scene);
        scene = NULL;
        goto END;
      }
      DX_UNREFERENCE(mesh_instance);
      mesh_instance = NULL;
    }
  }
END:
  return scene;
}

dx_asset_scene* dx_adl_parse_scene(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_scene* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  expected_type = NAME(scene_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_scene(node, state, names);
    if (!asset) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
