#include "dx/adl/semantical/mesh_instance_reader.h"

#include "dx/asset/mesh_instance.h"
#include "dx/adl/semantical/read.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}
#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

static dx_object* read(dx_adl_semantical_mesh_instance_reader*, dx_adl_node* node, dx_adl_semantical_state*);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_mesh_instance_reader",
                      dx_adl_semantical_mesh_instance_reader,
                      dx_adl_semantical_reader)

static dx_object* read(dx_adl_semantical_mesh_instance_reader* self, dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_mesh_instance* mesh_instance = NULL;
  dx_asset_mesh* mesh = NULL;
  DX_MAT4* transformation = NULL;
  // mesh
  {
    dx_string* name = NAME(mesh_key);
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (!child_node) {
      goto END;
    }
    if (child_node->kind != dx_adl_node_kind_map) {
      goto END;
    }
    dx_string* received_type = dx_adl_semantical_read_type(child_node, state->names);
    if (!dx_string_is_equal_to(received_type, NAME(mesh_type))) {
      goto END;
    }
    dx_adl_semantical_reader* mesh_reader = dx_pointer_hashmap_get(&state->readers, received_type);
    if (!mesh_reader) {
      goto END;
    }
    mesh = (dx_asset_mesh*)dx_adl_semantical_reader_read(mesh_reader, child_node, state);
    if (!mesh) {
      goto END;
    }
  }
  // transformation?
  {
    dx_string* name = dx_string_create("transformation", sizeof("transformation") - 1);
    if (!name) {
      goto END;
    }
    dx_error old_error = dx_get_error();
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      if (dx_get_error() == DX_NOT_FOUND) {
        dx_set_error(old_error);
      } else {
        goto END;
      }
    } else {
      transformation = dx_adl_parse_translation(child_node, state, state->names);
      if (!transformation) {
        goto END;
      }
    }
  }
  mesh_instance = dx_asset_mesh_instance_create(mesh);
  if (!mesh_instance) {
    goto END;
  }
  if (transformation) {
    mesh_instance->world_matrix = *transformation;
  }
END:
  if (mesh) {
    DX_UNREFERENCE(mesh);
    mesh = NULL;
  }
  if (transformation) {
    dx_memory_deallocate(transformation);
    transformation = NULL;
  }
  return DX_OBJECT(mesh_instance);
}

int dx_adl_semantical_mesh_instance_reader_construct(dx_adl_semantical_mesh_instance_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_mesh_instance_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_adl_node*, dx_adl_semantical_state*))&read;
  DX_OBJECT(self)->type = _type;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_adl_semantical_mesh_instance_reader_destruct;
  return 0;
}

void dx_adl_semantical_mesh_instance_reader_destruct(dx_adl_semantical_mesh_instance_reader* self) {
  dx_adl_semantical_reader_destruct(DX_ADL_SEMANTICAL_READER(self));
}

dx_adl_semantical_mesh_instance_reader* dx_adl_semantical_mesh_instance_reader_create() {
  dx_adl_semantical_mesh_instance_reader* self = DX_ADL_SEMANTICAL_MESH_INSTANCE_READER(dx_object_alloc(sizeof(dx_adl_semantical_mesh_instance_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_mesh_instance_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
