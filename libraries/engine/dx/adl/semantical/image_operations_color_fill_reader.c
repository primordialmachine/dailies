#include "dx/adl/semantical/image_operations_color_fill_reader.h"

#include "dx/adl/semantical/read.h"
#include "dx/asset/image_operations/color_fill.h"
#include "dx/adl/enter.h"
// strlen
#include <string.h>

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_semantical_name_index_##name)

static int resolve(dx_adl_semantical_image_operations_color_fill_reader* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_semantical_image_operations_color_fill_reader* self, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.image_operations_color_fill_reader",
                      dx_adl_semantical_image_operations_color_fill_reader,
                      dx_adl_semantical_reader)

static int resolve(dx_adl_semantical_image_operations_color_fill_reader* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  dx_asset_image_operations_color_fill* asset = DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL(symbol->asset);
  dx_ddl_node* node = symbol->node;
  // color
  {
    dx_asset_reference* color_reference = dx_adl_semantical_read_color_instance_field(node, false, NAME(color_key), context);
    if (!color_reference) {
      return 1;
    }
    dx_adl_symbol* color_symbol = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, color_reference->name));
    dx_asset_color* color_asset = DX_ASSET_COLOR(color_symbol->asset);
    DX_UNREFERENCE(color_reference);
    color_reference = NULL;
    //
    if (dx_asset_image_operations_color_fill_set_color(asset, color_asset)) {
      return 1;
    }
  }
  symbol->resolved = true;
  return 0;
}

static dx_object* read(dx_adl_semantical_image_operations_color_fill_reader* self, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  dx_asset_image_operations_color_fill* image_operation = dx_asset_image_operations_color_fill_create();
  if (!image_operation) {
    return NULL;
  }
  return DX_OBJECT(image_operation);
}

int dx_adl_semantical_image_operations_color_fill_reader_construct(dx_adl_semantical_image_operations_color_fill_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_image_operations_color_fill_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->resolve = (int(*)(dx_adl_semantical_reader*, dx_adl_symbol*, dx_adl_context*)) & resolve;
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object * (*)(dx_adl_semantical_reader*, dx_ddl_node*, dx_adl_context*)) & read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_image_operations_color_fill_reader_destruct(dx_adl_semantical_image_operations_color_fill_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_image_operations_color_fill_reader* dx_adl_semantical_image_operations_color_fill_reader_create() {
  dx_adl_semantical_image_operations_color_fill_reader* self = DX_ADL_SEMANTICAL_IMAGE_OPERATIONS_COLOR_FILL_READER(dx_object_alloc(sizeof(dx_adl_semantical_image_operations_color_fill_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_image_operations_color_fill_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
