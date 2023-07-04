#include "dx/adl/semantical/image_reader.h"

#include "dx/asset/image.h"
#include "dx/adl/semantical/read.h"
#include "dx/asset/image_operations/checkerboard_pattern_fill.h"
#include "dx/asset/image_operations/color_fill.h"
#include <string.h>

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
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

static dx_asset_image_operation* _read_image_operation(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_asset_image* _read_image(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_object* read(dx_adl_semantical_image_reader* self, dx_adl_node* node, dx_adl_semantical_state* state);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.image_reader",
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

static dx_asset_image_operation* _read_image_operation(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string *received_type = dx_adl_semantical_read_type(node, state);
  if (!received_type) {
    return NULL;
  }

  if (!dx_string_is_equal_to(received_type, NAME(image_operations_color_fill_type)) &&
      !dx_string_is_equal_to(received_type, NAME(image_operations_checkerboard_pattern_fill_type))) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return NULL;
  }
  
  dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&state->readers, received_type);
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  if (!reader) {
    return NULL;
  }

  dx_asset_image_operation* asset = DX_ASSET_IMAGE_OPERATION(dx_adl_semantical_reader_read(reader, node, state));
  if (!asset) {
    return NULL;
  }
  return asset;
}

static dx_asset_image* _read_image(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_image* image_value = NULL;
  dx_string* name_value = NULL;
  dx_size width_value, height_value;
  DX_RGB_U8* color_value = NULL;
  dx_asset_image_operation* image_operation = NULL;
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
  // operations
  {
    dx_string* name = NAME(operations_key);
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (!child_node) {
      goto END;
    }
    image_operation = _read_image_operation(child_node, state);
    if (!image_operation) {
      goto END;
    }
  }
  image_value = dx_asset_image_create(name_value, DX_PIXEL_FORMAT_RGB_U8, width_value, height_value, color_value);
  if (!image_value) {
    goto END;
  }
  if (dx_asset_image_apply(image_value, 0, 0, image_value->width, image_value->height, image_operation)) {
    goto END;
  }
END:
  if (name_value) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  }
  if (image_operation) {
    DX_UNREFERENCE(image_operation);
    image_operation = NULL;
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
