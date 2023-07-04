#include "dx/adl/semantical/image_operations_checkerboard_pattern_fill_reader.h"

#include "dx/adl/semantical/read.h"
#include "dx/asset/image_operations/checkerboard_pattern_fill.h"
// strlen
#include <string.h>

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

static int _read_color_instance(dx_adl_node* node, dx_adl_semantical_state* state, DX_RGB_U8* target);

static int _read_rgb_u8(dx_adl_node* node, char const* name, dx_adl_semantical_state* state, DX_RGB_U8* target);

static dx_object* read(dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader* self, dx_adl_node* node, dx_adl_semantical_state* state);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.image_operations_checkerboard_pattern_fill_reader",
                      dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader,
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

static dx_object* read(dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader* self, dx_adl_node* node, dx_adl_semantical_state* state) {
  if (!node) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  dx_asset_image_operations_checkerboard_pattern_fill* image_operation = dx_asset_image_operations_checkerboard_pattern_fill_create();
  if (!image_operation) {
    return NULL;
  }
  // numberOfCheckers
  {
    dx_string* child_name = dx_string_create("numberOfCheckers", sizeof("numberOfCheckers") - 1);
    if (!child_name) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, child_name);
    DX_UNREFERENCE(child_name);
    child_name = NULL;
    if (!child_node) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    // numberOfCheckers.horizontal
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(child_node, NAME(horizontal_key), &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_horizontal(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
    // numberOfCheckers.vertical
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(child_node, NAME(vertical_key), &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_vertical(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
  }
  // checkerSize
  {
    dx_string* child_name = dx_string_create("checkerSize", sizeof("checkerSize") - 1);
    if (!child_name) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, child_name);
    DX_UNREFERENCE(child_name);
    child_name = NULL;
    if (!child_node) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    // checkerSize.horizontal
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(child_node, NAME(horizontal_key), &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_horizontal(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
    // checkerSize.vertical
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(child_node, NAME(vertical_key), &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_vertical(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
  }
  // checkerColors
  {
    dx_asset_palette_entry* entry = NULL;
    dx_string* child_name = dx_string_create("checkerColors", sizeof("checkerColors") - 1);
    if (!child_name) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, child_name);
    DX_UNREFERENCE(child_name);
    child_name = NULL;
    if (!child_node) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    // checkerColors.first
    {
      DX_RGB_U8 value;
      if (_read_rgb_u8(child_node, "first", state, &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_first_checker_color(image_operation, &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
    // checkerColors.second
    {
      DX_RGB_U8 value;
      if (_read_rgb_u8(child_node, "second", state, &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_second_checker_color(image_operation, &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
  }
  return DX_OBJECT(image_operation);
}

int dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader_construct(dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object * (*)(dx_adl_semantical_reader*, dx_adl_node*, dx_adl_semantical_state*)) & read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader_destruct(dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader* dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader_create() {
  dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader* self = DX_ADL_SEMANTICAL_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_READER(dx_object_alloc(sizeof(dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
