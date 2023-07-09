#include "dx/adl/resolve.h"

#include "dx/adl/semantical/read.h"
#include "dx/asset/color.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(self->context->names, dx_semantical_name_index_##name)

DX_DEFINE_OBJECT_TYPE("dx.adl.resolve",
                      dx_adl_resolve,
                      dx_object)

static void dx_adl_resolve_destruct(dx_adl_resolve* self)
{/*Intentionally empty.*/}

int dx_adl_resolve_construct(dx_adl_resolve* self, dx_adl_context* context) {
  dx_rti_type* _type = dx_adl_resolve_get_type();
  if (!_type) {
    return 1;
  }
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->context = context;
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_adl_resolve* dx_adl_resolve_create(dx_adl_context* context) {
  dx_rti_type* _type = dx_adl_resolve_get_type();
  if (!_type) {
    return NULL;
  }
  dx_adl_resolve* self = DX_ADL_RESOLVE(dx_object_alloc(sizeof(dx_adl_resolve)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_resolve_construct(self, context)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_adl_resolve_do_symbol(dx_adl_resolve* self, dx_adl_symbol* symbol) {
  //
  if (!symbol->asset) {
    if (dx_string_is_equal_to(symbol->type, NAME(color_type))) {
      dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&self->context->readers, symbol->type);
      if (!reader) {
        return 1;
      }
      symbol->asset = dx_adl_semantical_reader_read(reader, symbol->node, self->context);
      if (!symbol->asset) {
        return 1;
      }
    }
  }
  //
  if (/*dx_string_is_equal_to(symbol->type, NAME(mesh_instance_type)) ||*/
    dx_string_is_equal_to(symbol->type, NAME(image_type)) ||
    dx_string_is_equal_to(symbol->type, NAME(mesh_type)) ||
    dx_string_is_equal_to(symbol->type, NAME(material_type)) ||
    dx_string_is_equal_to(symbol->type, NAME(texture_type))) {
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&self->context->readers, symbol->type);
    if (!reader) {
      return 1;
    }
    if (!symbol->asset) {
      symbol->asset = dx_adl_semantical_reader_read(reader, symbol->node, self->context);
      if (!symbol->asset) {
        return 1;
      }
    }
    if (dx_adl_semantical_reader_complete(reader, symbol, self->context)) {
      return 1;
    }
  }
  //
  return 0;
}

int dx_adl_resolve_do(dx_adl_resolve* self) {
  dx_size resolved;
  do {
    resolved = 0;
    dx_pointer_hashmap_iterator iterator;
    dx_pointer_hashmap_iterator_initialize(&iterator, &self->context->definitions->map);
    while (dx_pointer_hashmap_iterator_has_entry(&iterator)) {
      dx_adl_symbol* symbol = dx_pointer_hashmap_iterator_get_value(&iterator);
      if (!symbol->asset) {
        if (dx_string_is_equal_to(symbol->type, NAME(color_type))) {
          dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&self->context->readers, symbol->type);
          if (!reader) {
            dx_pointer_hashmap_iterator_uninitialize(&iterator);
            return 1;
          }
          symbol->asset = dx_adl_semantical_reader_read(reader, symbol->node, self->context);
          if (!symbol->asset) {
            dx_pointer_hashmap_iterator_uninitialize(&iterator);
            return 1;
          }
        }
      }
      dx_pointer_hashmap_iterator_next(&iterator);
    }
    dx_pointer_hashmap_iterator_uninitialize(&iterator);
  } while (resolved > 0);

  do {
    resolved = 0;
    dx_pointer_hashmap_iterator iterator;
    dx_pointer_hashmap_iterator_initialize(&iterator, &self->context->definitions->map);
    while (dx_pointer_hashmap_iterator_has_entry(&iterator)) {
      dx_adl_symbol* symbol = dx_pointer_hashmap_iterator_get_value(&iterator);
      //
      if (/*dx_string_is_equal_to(symbol->type, NAME(mesh_instance_type)) ||*/
        dx_string_is_equal_to(symbol->type, NAME(image_type)) ||
        dx_string_is_equal_to(symbol->type, NAME(mesh_type)) ||
        dx_string_is_equal_to(symbol->type, NAME(material_type)) ||
        dx_string_is_equal_to(symbol->type, NAME(texture_type))) {
        dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&self->context->readers, symbol->type);
        if (!reader) {
          dx_pointer_hashmap_iterator_uninitialize(&iterator);
          return 1;
        }
        if (!symbol->asset) {
          symbol->asset = dx_adl_semantical_reader_read(reader, symbol->node, self->context);
          if (!symbol->asset) {
            dx_pointer_hashmap_iterator_uninitialize(&iterator);
            return 1;
          }
        }
        if (dx_adl_semantical_reader_complete(reader, symbol, self->context)) {
          dx_pointer_hashmap_iterator_uninitialize(&iterator);
          return 1;
        }
      }
      dx_pointer_hashmap_iterator_next(&iterator);
    }
    dx_pointer_hashmap_iterator_uninitialize(&iterator);
  } while (resolved > 0);

  return 0;
}