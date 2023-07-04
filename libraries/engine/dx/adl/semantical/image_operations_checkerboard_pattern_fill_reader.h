#if !defined(DX_ADL_SEMANTICAL_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_READER_H_INCLUDED)
#define DX_ADL_SEMANTICAL_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_READER_H_INCLUDED

#include "dx/adl/semantical/state.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.semantical.image_operations_checkerboard_pattern_fill_reader",
                       dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader,
                       dx_adl_semantical_reader)

static inline dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader* DX_ADL_SEMANTICAL_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_READER(void* p) {
  return (dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader*)p;
}

struct dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader {
  dx_adl_semantical_reader _parent;
};

int dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader_construct(dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader* self);

dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader* dx_adl_semantical_image_operations_checkerboard_pattern_fill_reader_create();

#endif // DX_ADL_SEMANTICAL_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_READER_H_INCLUDED
