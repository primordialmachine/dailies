#include "dx/adl/semantical/names.h"

// strlen
#include <string.h>

int dx_adl_semantical_names_construct(dx_adl_semantical_names* self) {
  self->names = (dx_string**)dx_memory_allocate(sizeof(dx_string*)*(DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES));
  if (!self->names) {
    goto ERROR;
  }
  memset(self->names, 0, sizeof(dx_string*) * (DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES));
#define DEFINE(NAME, NUMBER, STRING) \
  self->names[dx_semantical_name_index_##NAME] = dx_string_create(STRING, sizeof(STRING) - 1); \
  if (!self->names[dx_semantical_name_index_##NAME]) { \
    goto ERROR; \
  }
#include "dx/adl/semantical/names.i"
#undef DEFINE
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_adl_semantical_names_destruct;
  return 0;
ERROR:
  if (self->names) {
    for (size_t i = 0, n = dx_semantical_name_index_z_key + 1; i < n; ++i) {
      if (self->names[i]) {
        DX_UNREFERENCE(self->names[i]);
        self->names[i] = NULL;
      }
    }
  }
  if (self->names) {
    dx_memory_deallocate(self->names);
    self->names = NULL;
  }
  return 1;
}

void dx_adl_semantical_names_destruct(dx_adl_semantical_names* self) {
  for (size_t i = 0, n = DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES; i < n; ++i) {
    DX_DEBUG_ASSERT(self->names[i]);
    DX_UNREFERENCE(self->names[i]);
    self->names[i] = NULL;
  }
  if (self->names) {
    dx_memory_deallocate(self->names);
    self->names = NULL;
  }
}

dx_adl_semantical_names* dx_adl_semantical_names_create() {
  dx_adl_semantical_names* self = DX_ADL_SEMANTICAL_NAMES(dx_object_alloc(sizeof(dx_adl_semantical_names)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_names_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
