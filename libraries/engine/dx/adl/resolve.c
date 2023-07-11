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

  static void dx_adl_resolve_destruct(dx_adl_resolve* self) {
  dx_pointer_array_uninitialize(&self->queue);
}

int dx_adl_resolve_construct(dx_adl_resolve* self, dx_adl_context* context) {
  dx_rti_type* _type = dx_adl_resolve_get_type();
  if (!_type) {
    return 1;
  }
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (dx_pointer_array_initialize(&self->queue, 0, NULL, NULL)) {
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

static int setup_queue(dx_adl_resolve* self, bool include_unloaded, bool include_unresolved) {
  dx_pointer_array_clear(&self->queue);
  dx_pointer_hashmap_iterator iterator;
  dx_pointer_hashmap_iterator_initialize(&iterator, &self->context->definitions->map);
  while (dx_pointer_hashmap_iterator_has_entry(&iterator)) {
    dx_adl_symbol* symbol = dx_pointer_hashmap_iterator_get_value(&iterator);
    if ((!symbol->asset && include_unloaded) || (!symbol->resolved && include_unresolved)) {
      if (dx_pointer_array_append(&self->queue, symbol)) {
        dx_pointer_hashmap_iterator_uninitialize(&iterator);
        return 1;
      }
    }
    dx_pointer_hashmap_iterator_next(&iterator);
  }
  dx_pointer_hashmap_iterator_uninitialize(&iterator);
  return 0;
}

int dx_adl_resolve_run(dx_adl_resolve* self) {
  // This reads all the top-level aka named aka referencable elements.
  // <root>
  //  <top-level-element>
  if (setup_queue(self, true, false)) {
    return 1;
  }
  for (dx_size i = 0, n = dx_pointer_array_get_size(&self->queue); i < n; ++i) {
    dx_adl_symbol* symbol = DX_ADL_SYMBOL(dx_pointer_array_get_at(&self->queue, i));
    if (!symbol->asset) {
      /*if (dx_string_is_equal_to(symbol->type, NAME(color_type)))*/ {
        dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&self->context->readers, symbol->type);
        if (!reader) {
          return 1;
        }
        symbol->asset = dx_adl_semantical_reader_read(reader, symbol->node, self->context);
        if (!symbol->asset) {
          return 1;
        } // if
      } // if
    } //if
  } // for

  // resolve references/read non-top-level elements
  do {
    if (setup_queue(self, true, true)) {
      return 1;
    } // if
    for (dx_size i = 0, n = dx_pointer_array_get_size(&self->queue); i < n; ++i) {
      dx_adl_symbol* symbol = DX_ADL_SYMBOL(dx_pointer_array_get_at(&self->queue, i));
      dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&self->context->readers, symbol->type);
      if (!reader) {
        return 1;
      }
      if (!symbol->asset) {
        continue;/*Proceed with speculative (the program is invalid) execution.*/
      }
      if (!symbol->asset) {
        symbol->asset = dx_adl_semantical_reader_read(reader, symbol->node, self->context);
        if (!symbol->asset) {
          return 1;
        }
      }
      if (dx_adl_semantical_reader_resolve(reader, symbol, self->context)) {
        return 1;
      }
    } // for
  } while (dx_pointer_array_get_size(&self->queue));

  return 0;
}
