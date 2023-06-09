#if !defined(DX_ADL_SEMANTICAL_MESH_OPERATIONS_READER_H_INCLUDED)
#define DX_ADL_SEMANTICAL_MESH_OPERATIONS_READER_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.semantical_mesh_operations_reader",
                       dx_adl_semantical_mesh_operations_reader,
                       dx_adl_semantical_reader)

static inline dx_adl_semantical_mesh_operations_reader* DX_ADL_SEMANTICAL_MESH_OPERATIONS_READER(void* p) {
  return (dx_adl_semantical_mesh_operations_reader*)p;
}

struct dx_adl_semantical_mesh_operations_reader {
  dx_adl_semantical_reader _parent;
};

int dx_adl_semantical_mesh_operations_reader_construct(dx_adl_semantical_mesh_operations_reader* self);

dx_adl_semantical_mesh_operations_reader* dx_adl_semantical_mesh_operations_reader_create();

#endif // DX_ADL_SEMANTICAL_MESH_OPERATIONS_READER_H_INCLUDED
