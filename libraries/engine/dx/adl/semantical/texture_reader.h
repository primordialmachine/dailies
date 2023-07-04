#if !defined(DX_ADL_SEMANTICAL_TEXTURE_READER_H_INCLUDED)
#define DX_ADL_SEMANTICAL_TEXTURE_READER_H_INCLUDED

#include "dx/adl/semantical/state.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.semantical.texture_reader",
                       dx_adl_semantical_texture_reader,
                       dx_adl_semantical_reader)

static inline dx_adl_semantical_texture_reader* DX_ADL_SEMANTICAL_TEXTURE_READER(void* p) {
  return (dx_adl_semantical_texture_reader*)p;
}

struct dx_adl_semantical_texture_reader {
  dx_adl_semantical_reader _parent;
};

int dx_adl_semantical_texture_reader_construct(dx_adl_semantical_texture_reader* self);

dx_adl_semantical_texture_reader* dx_adl_semantical_texture_reader_create();

#endif // DX_ADL_SEMANTICAL_TEXTURE_READER_H_INCLUDED
