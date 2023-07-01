#include "dx/adl/semantical/image_reader.h"

#include "dx/asset/image.h"
#include "dx/adl/semantical/read.h"
#include <string.h>

static inline dx_string* _get_name(dx_adl_semantical_names* names, size_t index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int _read_color_instance(dx_adl_node* node, dx_adl_semantical_state* state, DX_RGB_U8* target);

static int _read_rgb_u8(dx_adl_node* node, char const* name, dx_adl_semantical_state* state, DX_RGB_U8* target);

static int _read_solid_brush_1(DX_ASSET_SOLID_BRUSH* target, dx_adl_node* node, dx_adl_semantical_state* state);

static int _read_checkerboard_brush_1(DX_ASSET_CHECKERBOARD_BRUSH* target, dx_adl_node* node, dx_adl_semantical_state* state);

static DX_ASSET_SOLID_BRUSH* _read_solid_brush(dx_adl_node* node, dx_adl_semantical_state* state);

static DX_ASSET_CHECKERBOARD_BRUSH* _read_checkerboard_brush(dx_adl_node* node, dx_adl_semantical_state* state);

static DX_ASSET_BRUSH* _read_brush(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_asset_image* _read_image(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_object* read(dx_adl_semantical_image_reader*, dx_adl_node* node, dx_adl_semantical_state*);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_image_reader",
                      dx_adl_semantical_image_reader,
                      dx_adl_semantical_reader)

static int _read_color_instance(dx_adl_node* node, dx_adl_semantical_state* state, DX_RGB_U8* target) {
  dx_string* expected_type = NAME(color_instance_type);
  dx_string* received_type = dx_adl_semantical_read_type(node, state);
  if (!received_type) {
    return 1;
  }
  if (!dx_string_is_equal_to(received_type, expected_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_SEMANTICAL_ERROR);
    return 1;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  dx_string* value = dx_adl_semantical_read_string(node, NAME(reference_key), state->names);
  if (!value) {
    return 1;
  }
  dx_asset_palette_entry* palette_entry = dx_asset_palette_get(state->scene->palette, value);
  DX_UNREFERENCE(value);
  value = NULL;
  if (!palette_entry) {
    return 1;
  }
  (*target) = palette_entry->value;
  return 0;
}

static int _read_rgb_u8(dx_adl_node* node, char const* name, dx_adl_semantical_state* state, DX_RGB_U8* target) {
  dx_string* name1 = dx_string_create(name, strlen(name));
  if (!name1) {
    return 1;
  }
  dx_adl_node* child_node = dx_adl_node_map_get(node, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  return _read_color_instance(child_node, state, target);
}

static int _read_solid_brush_1(DX_ASSET_SOLID_BRUSH* target, dx_adl_node* node, dx_adl_semantical_state* state) {
  if (!node) {
    goto END;
  }
  if (_read_rgb_u8(node, "color", state, &target->color)) {
    goto END;
  }
END:
  return 0;
}

static int _read_checkerboard_brush_1(DX_ASSET_CHECKERBOARD_BRUSH* target, dx_adl_node* root_node, dx_adl_semantical_state* state) {
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
    if (dx_adl_semantical_read_sz(number_of_checkers_node, NAME(horizontal_key), &target->number_of_checkers.horizontal)) {
      goto END;
    }
    // numberOfCheckers.vertical
    if (dx_adl_semantical_read_sz(number_of_checkers_node, NAME(vertical_key), &target->number_of_checkers.vertical)) {
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
    if (dx_adl_semantical_read_sz(checker_size_node, NAME(horizontal_key), &target->checker_size.horizontal)) {
      goto END;
    }
    // checkerSize.vertical
    if (dx_adl_semantical_read_sz(checker_size_node, NAME(vertical_key), &target->checker_size.vertical)) {
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
    if (_read_rgb_u8(checker_colors_node, "first", state, &target->checker_colors.first)) {
      goto END;
    }
    // checkerColors.second
    if (_read_rgb_u8(checker_colors_node, "second", state, &target->checker_colors.second)) {
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

static DX_ASSET_SOLID_BRUSH* _read_solid_brush(dx_adl_node* node, dx_adl_semantical_state* state) {
  DX_ASSET_SOLID_BRUSH* brush = dx_memory_allocate(sizeof(DX_ASSET_SOLID_BRUSH));
  if (!brush) {
    return NULL;
  }
  dx_memory_zero(brush, sizeof(DX_ASSET_SOLID_BRUSH));
  brush->_parent.flags = DX_ASSET_BRUSH_FLAGS_SOLID;
  if (_read_solid_brush_1(brush, node, state)) {
    dx_memory_deallocate(brush);
    brush = NULL;
    return NULL;
  }
  return brush;
}

static DX_ASSET_CHECKERBOARD_BRUSH* _read_checkerboard_brush(dx_adl_node* node, dx_adl_semantical_state* state) {
  DX_ASSET_CHECKERBOARD_BRUSH* brush = dx_memory_allocate(sizeof(DX_ASSET_CHECKERBOARD_BRUSH));
  if (!brush) {
    return NULL;
  }
  dx_memory_zero(brush, sizeof(DX_ASSET_CHECKERBOARD_BRUSH));
  brush->_parent.flags = DX_ASSET_BRUSH_FLAGS_CHECKBERBOARD;
  if (_read_checkerboard_brush_1(brush, node, state)) {
    dx_memory_deallocate(brush);
    brush = NULL;
    return NULL;
  }
  return brush;
}

static DX_ASSET_BRUSH* _read_brush(dx_adl_node* node, dx_adl_semantical_state* state) {
  DX_ASSET_BRUSH* brush = NULL;
  dx_string* solid_type = NULL;
  dx_string* checkerboard_type = NULL;
  dx_string* received_type = NULL;

  received_type = dx_adl_semantical_read_type(node, state);
  if (!received_type) {
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

  if (dx_string_is_equal_to(received_type, solid_type)) {
    brush = (DX_ASSET_BRUSH*)_read_solid_brush(node, state);
    if (!brush) {
      goto END;
    }
  } else if (dx_string_is_equal_to(received_type, checkerboard_type)) {
    brush = (DX_ASSET_BRUSH*)_read_checkerboard_brush(node, state);
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
  if (received_type) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
  }
  return brush;
}

static dx_asset_image* _read_image(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_image* image_value = NULL;
  dx_string* name_value = NULL;
  dx_size width_value, height_value;
  DX_RGB_U8* color_value = NULL;
  DX_ASSET_BRUSH* brush_value = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, state);
    if (!name_value) {
      goto END;
    }
  }
  // width
  if (dx_adl_semantical_read_sz(node, NAME(width_key), &width_value)) {
    goto END;
  }
  // height
  if (dx_adl_semantical_read_sz(node, NAME(height_key), &height_value)) {
    goto END;
  }
  // color
  {
    color_value = dx_memory_allocate(sizeof(DX_RGB_U8));
    if (!color_value) {
      goto END;
    }
    if (_read_rgb_u8(node, "color", state, color_value)) {
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
    brush_value = _read_brush(child_node, state);
    if (!brush_value) {
      goto END;
    }
  }
  image_value = dx_asset_image_create(name_value, DX_PIXEL_FORMAT_RGB_U8, width_value, height_value, color_value);
  if (!image_value) {
    goto END;
  }
  if (dx_asset_image_fill(image_value, 0, 0, image_value->width, image_value->height, brush_value)) {
    goto END;
  }
END:
  if (name_value) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  }
  if (brush_value) {
    dx_memory_deallocate(brush_value);
    brush_value = NULL;
  }
  if (color_value) {
    dx_memory_deallocate(color_value);
    color_value = NULL;
  }
  return image_value;
}

static dx_object* read(dx_adl_semantical_image_reader* self, dx_adl_node* node, dx_adl_semantical_state* state) {
  return DX_OBJECT(_read_image(node, state));
}

int dx_adl_semantical_image_reader_construct(dx_adl_semantical_image_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_image_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_adl_node*, dx_adl_semantical_state*))&read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_image_reader_destruct(dx_adl_semantical_image_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_image_reader* dx_adl_semantical_image_reader_create() {
  dx_adl_semantical_image_reader* self = DX_ADL_SEMANTICAL_IMAGE_READER(dx_object_alloc(sizeof(dx_adl_semantical_image_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_image_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
