#include "dx/adl/semantical/color_reader.h"

#include "dx/asset/color.h"
#include "dx/adl/semantical/read.h"
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

static int _read_color_rgb_u8(dx_ddl_node* node, dx_adl_context* context, DX_RGB_U8* target);

static dx_object* read(dx_adl_semantical_color_reader* self, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.color_reader",
  dx_adl_semantical_color_reader,
  dx_adl_semantical_reader)

  static int _read_color_rgb_u8(dx_ddl_node* node, dx_adl_context* context, DX_RGB_U8* target) {
  dx_n8 r, g, b;
  if (dx_adl_semantical_read_n8(node, NAME(red_key), &r)) {
    return 1;
  }
  if (dx_adl_semantical_read_n8(node, NAME(green_key), &g)) {
    return 1;
  }
  if (dx_adl_semantical_read_n8(node, NAME(blue_key), &b)) {
    return 1;
  }
  // TODO: Check bounds.
  target->r = r;
  target->g = g;
  target->b = b;
  return 0;
}

static dx_object* read(dx_adl_semantical_color_reader* self, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  DX_RGB_U8 temporary;
  if (_read_color_rgb_u8(node, context, &temporary)) {
    return NULL;
  }
  dx_asset_color* color = dx_asset_color_create(&temporary);
  if (!color) {
    return NULL;
  }
  return DX_OBJECT(color);
}

int dx_adl_semantical_color_reader_construct(dx_adl_semantical_color_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_color_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object * (*)(dx_adl_semantical_reader*, dx_ddl_node*, dx_adl_context*)) & read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_color_reader_destruct(dx_adl_semantical_color_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_color_reader* dx_adl_semantical_color_reader_create() {
  dx_adl_semantical_color_reader* self = DX_ADL_SEMANTICAL_COLOR_READER(dx_object_alloc(sizeof(dx_adl_semantical_color_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_color_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
