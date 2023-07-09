#include "dx/adl/semantical/image_reader.h"

#include "dx/asset/image.h"
#include "dx/adl/semantical/read.h"
#include "dx/asset/image_operations/checkerboard_pattern_fill.h"
#include "dx/asset/image_operations/color_fill.h"
#include "dx/adl/enter.h"
#include <string.h>

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_image_operation* _read_image_operation(dx_ddl_node* node, dx_adl_context* context);

static dx_asset_image* _read_image(dx_ddl_node* node, dx_adl_context* context);

static int complete(dx_adl_semantical_image_reader* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_semantical_image_reader* self, dx_ddl_node* node, dx_adl_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.image_reader",
                      dx_adl_semantical_image_reader,
                      dx_adl_semantical_reader)

static dx_asset_image_operation* _read_image_operation(dx_ddl_node* node, dx_adl_context* context) {
  dx_string *received_type = dx_adl_semantical_read_type(node, context);
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
  
  dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, received_type);
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  if (!reader) {
    return NULL;
  }

  dx_asset_image_operation* asset = DX_ASSET_IMAGE_OPERATION(dx_adl_semantical_reader_read(reader, node, context));
  if (!asset) {
    return NULL;
  }
  return asset;
}

static dx_asset_image* _read_image(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_image* image_value = NULL;
  dx_string* name_value = NULL;
  dx_size width_value, height_value;
  dx_asset_color* color_value = NULL;
  dx_asset_image_operation* image_operation = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, context);
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
    color_value = dx_adl_semantical_read_color_instance_field(node, false, NAME(color_key), context);
    if (!color_value) {
      goto END;
    }
  }
  // operations
  {
    dx_string* name = NAME(operations_key);
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
    if (!child_node) {
      goto END;
    }
    image_operation = _read_image_operation(child_node, context);
    if (!image_operation) {
      goto END;
    }
  }
  image_value = dx_asset_image_create(name_value, DX_PIXEL_FORMAT_RGB_U8, width_value, height_value, &color_value->value);
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
    DX_UNREFERENCE(color_value);
    color_value = NULL;
  }
  return image_value;
}

static int complete(dx_adl_semantical_image_reader* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  return 0;
}

static dx_object* read(dx_adl_semantical_image_reader* self, dx_ddl_node* node, dx_adl_context* context) {
  return DX_OBJECT(_read_image(node, context));
}

int dx_adl_semantical_image_reader_construct(dx_adl_semantical_image_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_image_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->complete = (int(*)(dx_adl_semantical_reader*, dx_adl_symbol*, dx_adl_context*))&complete;
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_ddl_node*, dx_adl_context*))&read;
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
