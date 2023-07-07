#include "dx/adl/semantical/mesh_operations_reader.h"

#include "dx/asset/mesh_operations/set_vertex_colors.h"
#include "dx/adl/semantical/read.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_semantical_name_index_##name)

static dx_object* read(dx_adl_semantical_mesh_operations_reader* self,
                       dx_ddl_node* node,
                       dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.mesh_operations_reader",
                      dx_adl_semantical_mesh_operations_reader,
                      dx_adl_semantical_reader)

static dx_object* read(dx_adl_semantical_mesh_operations_reader* self, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(mesh_operations_set_vertex_colors_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_OBJECT(dx_asset_mesh_operations_set_vertex_colors_create());
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
}

int dx_adl_semantical_mesh_operations_reader_construct(dx_adl_semantical_mesh_operations_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_mesh_operations_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object * (*)(dx_adl_semantical_reader*, dx_ddl_node*, dx_adl_context*)) & read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_mesh_operations_reader_destruct(dx_adl_semantical_mesh_operations_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_mesh_operations_reader* dx_adl_semantical_mesh_operations_reader_create() {
  dx_adl_semantical_mesh_operations_reader* self = DX_ADL_SEMANTICAL_MESH_OPERATIONS_READER(dx_object_alloc(sizeof(dx_adl_semantical_mesh_operations_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_mesh_operations_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
