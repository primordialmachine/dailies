#include "dx/adl/semantical/mesh_reader.h"

static dx_object* read(dx_adl_semantical_mesh_reader*,
                       dx_adl_node* node,
                       dx_adl_semantical_state*,
                       dx_adl_semantical_names*);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_mesh_reader",
                      dx_adl_semantical_mesh_reader,
                      dx_adl_semantical_reader)

static dx_object* read(dx_adl_semantical_mesh_reader* self, dx_adl_node* node,
                       dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  return 0;
}

int dx_adl_semantical_mesh_reader_construct(dx_adl_semantical_mesh_reader* self) {
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_adl_node*, dx_adl_semantical_state*, dx_adl_semantical_names*))&read;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_adl_semantical_mesh_reader_destruct;
  return 0;
}

void dx_adl_semantical_mesh_reader_destruct(dx_adl_semantical_mesh_reader* self) {
  dx_adl_semantical_reader_destruct(DX_ADL_SEMANTICAL_READER(self));
}

dx_adl_semantical_mesh_reader* dx_adl_semantical_mesh_reader_create() {
  dx_adl_semantical_mesh_reader* self = DX_ADL_SEMANTICAL_MESH_READER(dx_object_alloc(sizeof(dx_adl_semantical_mesh_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_mesh_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
