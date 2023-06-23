#if !defined(DX_ADL_SEMANTICAL_NAMES_COLOR_H_INCLUDED)
#define DX_ADL_SEMANTICAL_NAMES_COLOR_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_ENUMERATION_TYPE("dx.adl.semantical_name_index",
                            dx_adl_semantical_name_index)

  enum dx_semantical_name_index {
#define DEFINE(NAME, NUMBER, STRING) dx_semantical_name_index_##NAME = NUMBER,
#include "dx/adl/semantical/names.i"
#undef DEFINE
};

#define DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES (((dx_size)dx_semantical_name_index_z_key) + 1)

DX_DECLARE_OBJECT_TYPE("dx.adl.semantical_names",
                       dx_adl_semantical_names,
                       dx_object)

static inline dx_adl_semantical_names* DX_ADL_SEMANTICAL_NAMES(void* p) {
  return (dx_adl_semantical_names*)p;
}

struct dx_adl_semantical_names {
  dx_object parent;
  dx_string** names;
};

int dx_adl_semantical_names_construct(dx_adl_semantical_names* self);

void dx_adl_semantical_names_destruct(dx_adl_semantical_names* self);

dx_adl_semantical_names* dx_adl_semantical_names_create();

#endif // DX_ADL_SEMANTICAL_NAMES_COLOR_H_INCLUDED
