#if !defined(DX_ADL_SEMANTICAL_MESH_READER_H_INCLUDED)
#define DX_ADL_SEMANTICAL_MESH_READER_H_INCLUDED

#include "dx/adl/semantical/state.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.semantical.mesh_reader",
                       dx_adl_semantical_mesh_reader,
                       dx_adl_semantical_reader)

static inline dx_adl_semantical_mesh_reader* DX_ADL_SEMANTICAL_MESH_READER(void* p) {
  return (dx_adl_semantical_mesh_reader*)p;
}

struct dx_adl_semantical_mesh_reader {
  dx_adl_semantical_reader _parent;
};

int dx_adl_semantical_mesh_reader_construct(dx_adl_semantical_mesh_reader* self);

dx_adl_semantical_mesh_reader* dx_adl_semantical_mesh_reader_create();

#endif // DX_ADL_SEMANTICAL_MESH_READER_H_INCLUDED
