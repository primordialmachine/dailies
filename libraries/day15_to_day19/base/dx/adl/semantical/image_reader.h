#if !defined(DX_ADL_SEMANTICAL_MATERIAL_READER_H_INCLUDED)
#define DX_ADL_SEMANTICAL_MATERIAL_READER_H_INCLUDED

#include "dx/adl/semantical/state.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.semantical_image_reader",
                       dx_adl_semantical_image_reader,
                       dx_adl_semantical_reader)

static inline dx_adl_semantical_image_reader* DX_ADL_SEMANTICAL_IMAGE_READER(void* p) {
  return (dx_adl_semantical_image_reader*)p;
}

struct dx_adl_semantical_image_reader {
  dx_adl_semantical_reader _parent;
};

int dx_adl_semantical_image_reader_construct(dx_adl_semantical_image_reader* self);

void dx_adl_semantical_image_reader_destruct(dx_adl_semantical_image_reader* self);

dx_adl_semantical_image_reader* dx_adl_semantical_image_reader_create();

#endif // DX_ADL_SEMANTICAL_IMAGE_READER_H_INCLUDED
