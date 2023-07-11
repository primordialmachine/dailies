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

static int _read_image_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static int _read_image_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static void on_object_added(dx_object** o);

static void on_object_removed(dx_object** o);

static dx_asset_image* _read_image(dx_ddl_node* node, dx_adl_context* context);

static int resolve(dx_adl_semantical_image_reader* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_semantical_image_reader* self, dx_ddl_node* node, dx_adl_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.image_reader",
                      dx_adl_semantical_image_reader,
                      dx_adl_semantical_reader)

static int _read_image_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return 1;
  }

  if (!dx_string_is_equal_to(received_type, NAME(image_operations_color_fill_type)) &&
      !dx_string_is_equal_to(received_type, NAME(image_operations_checkerboard_pattern_fill_type))) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return 1;
  }

  dx_adl_symbol* reader_symbol = dx_adl_symbol_create(received_type, dx_adl_semantical_names_create_unique_name(context->names));
  if (!reader_symbol) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return 1;
  }
  if (dx_asset_definitions_set(context->definitions, reader_symbol->name, reader_symbol)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return 0;
  }

  dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, received_type);
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  if (!reader) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return 1;
  }

  dx_asset_image_operation* operation = DX_ASSET_IMAGE_OPERATION(dx_adl_semantical_reader_read(reader, node, context));
  if (!operation) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return 1;
  }
  reader_symbol->asset = DX_OBJECT(operation);
  DX_REFERENCE(reader_symbol->asset);
  DX_UNREFERENCE(reader_symbol);
  reader_symbol = NULL;
  dx_asset_image* image = DX_ASSET_IMAGE(symbol->asset);
  if (dx_object_array_append(&image->operations, DX_OBJECT(operation))) {
    DX_UNREFERENCE(operation);
    operation = NULL;
    return 1;
  }
  DX_UNREFERENCE(operation);
  operation = NULL;
  return 0;
}

static void on_object_added(dx_object** o) {
  DX_REFERENCE(*o);
}

static void on_object_removed(dx_object** o) {
  DX_UNREFERENCE(*o);
}

static int _read_image_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (node->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return 1;
  }
  for (dx_size i = 0, n = dx_ddl_node_list_get_size(node); i < n; ++i) {
    dx_ddl_node* child_node = dx_ddl_node_list_get(node, i);
    if (!child_node) {
      return 1;
    }
    if (_read_image_operation(child_node, symbol, context)) {
      return 1;
    }
  }
  return 0;
}

static dx_asset_image* _read_image(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_image* image_value = NULL;
  dx_string* name_value = NULL;
  dx_size width_value, height_value;
  dx_asset_color* color_value = NULL;
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
  DX_RGB_U8 white = { .r = 255, .g = 255, .b = 255 };
  image_value = dx_asset_image_create(name_value, DX_PIXEL_FORMAT_RGB_U8, width_value, height_value, &white);
  if (!image_value) {
    goto END;
  }
END:
  if (name_value) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  }
  if (color_value) {
    DX_UNREFERENCE(color_value);
    color_value = NULL;
  }
  return image_value;
}

static int resolve(dx_adl_semantical_image_reader* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  dx_asset_image* image = DX_ASSET_IMAGE(symbol->asset);
  // operations
  {
    dx_error last_error = dx_get_error();
    dx_string* name = NAME(operations_key);
    dx_ddl_node* child_node = dx_ddl_node_map_get(symbol->node, name);
    if (child_node) {
      if (_read_image_operations(child_node, symbol, context)) {
        return 1;
      }
    } else {
      if (dx_get_error() != DX_NOT_FOUND) {
        return 1;
      } else {
        dx_set_error(last_error);
      }
    }
  }
  symbol->resolved = true;
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
  DX_ADL_SEMANTICAL_READER(self)->resolve = (int(*)(dx_adl_semantical_reader*, dx_adl_symbol*, dx_adl_context*))&resolve;
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
