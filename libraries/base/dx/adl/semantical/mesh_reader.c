#include "dx/adl/semantical/mesh_reader.h"

#include "dx/asset/mesh.h"
#include "dx/adl/semantical/read.h"
#include "dx/asset/mesh_operation.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, size_t index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}
#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

static dx_asset_material* _read_material(dx_adl_node* node, dx_adl_semantical_state* state);

static void on_object_added(dx_object** o);

static void on_object_removed(dx_object** o);

static int _read_mesh_operation(dx_adl_node* node, dx_adl_semantical_state* state, dx_pointer_array* mesh_operations);

static dx_pointer_array* _read_mesh_operations(dx_adl_node* node, dx_adl_semantical_state* state);

static int _read_vertex_format(dx_adl_node* node, dx_adl_semantical_state* state, DX_VERTEX_FORMAT* vertex_format);

static dx_asset_mesh* _read_mesh(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_object* read(dx_adl_semantical_mesh_reader*,
                       dx_adl_node* node,
                       dx_adl_semantical_state*);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_mesh_reader",
                      dx_adl_semantical_mesh_reader,
                      dx_adl_semantical_reader)

static dx_asset_material* _read_material(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_material* asset = NULL;
  dx_string* received_type = dx_adl_semantical_read_type(node, state->names);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(material_type))) {
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&state->readers, NAME(material_type));
    if (!reader) {
      return NULL;
    }
    asset = DX_ASSET_MATERIAL(dx_adl_semantical_reader_read(reader, node, state));
    if (!asset) {
      goto END;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  return asset;
}

static void on_object_added(dx_object** o) {
  DX_REFERENCE(*o);
}

static void on_object_removed(dx_object** o) {
  DX_UNREFERENCE(*o);
}

static int _read_mesh_operation(dx_adl_node* node, dx_adl_semantical_state* state, dx_pointer_array* mesh_operations) {
  if (node->kind != dx_adl_node_kind_map) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return 1;
  }
  dx_string* received_type = dx_adl_semantical_read_type(node, state->names);
  if (!received_type) {
    return 1;
  }
  dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&state->readers, received_type);
  if (!reader) {
    if (dx_get_error() != DX_NOT_FOUND) {
      return 1;
    } else {
      dx_set_error(DX_SEMANTICAL_ERROR);
      return 1;
    }
  }
  /// @todo Check type of return value.
  dx_asset_mesh_operation* mesh_operation = DX_ASSET_MESH_OPERATION(dx_adl_semantical_reader_read(reader, node, state));
  if (!mesh_operation) {
    return 1;
  }
  if (dx_pointer_array_append(mesh_operations, mesh_operation)) {
    DX_UNREFERENCE(mesh_operation);
    mesh_operation = NULL;
    return 1;
  }
  DX_UNREFERENCE(mesh_operation);
  mesh_operation = NULL;
  return 0;
}

static dx_pointer_array* _read_mesh_operations(dx_adl_node* node, dx_adl_semantical_state* state) {
  if (node->kind != dx_adl_node_kind_list) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
  dx_pointer_array* mesh_operations = dx_memory_allocate(sizeof(dx_pointer_array));
  if (!mesh_operations) {
    return NULL;
  }
  if (dx_pointer_array_initialize(mesh_operations, 0,
                                  (dx_added_callback*)&on_object_added,
                                  (dx_removed_callback*)&on_object_removed)) {
    dx_memory_deallocate(mesh_operations);
    mesh_operations = NULL;
    return NULL;
  }
  for (dx_size i = 0, n = dx_adl_node_list_get_size(node); i < n; ++i) {
    dx_adl_node* child_node = dx_adl_node_list_get(node, i);
    if (!child_node) {
      dx_pointer_array_uninitialize(mesh_operations);
      dx_memory_deallocate(mesh_operations);
      mesh_operations = NULL;
      return NULL;
    }
    if (_read_mesh_operation(child_node, state, mesh_operations)) {
      dx_pointer_array_uninitialize(mesh_operations);
      dx_memory_deallocate(mesh_operations);
      mesh_operations = NULL;
      return NULL;
    }
  }
  return mesh_operations;
}

static int _read_vertex_format(dx_adl_node* node, dx_adl_semantical_state* state, DX_VERTEX_FORMAT* vertex_format) {
  if (!node || node->kind != dx_adl_node_kind_list) {
    return 1;
  }
  DX_VERTEX_FORMAT vertex_format_1 = 0;
  for (dx_size i = 0, n = dx_adl_node_list_get_size(node); i < n; ++i) {
    dx_adl_node* child_node = dx_adl_node_list_get(node, i);
    dx_string* received_value = dx_adl_node_get_string(child_node);
    if (dx_string_is_equal_to(received_value, NAME(position_xyz_string))) {
      vertex_format_1 |= DX_VERTEX_FORMAT_POSITION_XYZ;
    } else if (dx_string_is_equal_to(received_value, NAME(ambient_rgba_string))) {
      vertex_format_1 |= DX_VERTEX_FORMAT_AMBIENT_RGBA;
    } else if (dx_string_is_equal_to(received_value, NAME(ambient_uv_string))) {
      vertex_format_1 |= DX_VERTEX_FORMAT_AMBIENT_UV;
    } else {
      return 1;
    }
  }
  (*vertex_format) = vertex_format_1;
  return 0;
}

static dx_asset_mesh* _read_mesh(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_mesh* mesh = NULL;
  dx_string* generator = NULL;
  dx_asset_material* material = NULL;
  dx_pointer_array* mesh_operations = NULL;

  // generator
  {
    dx_string* name = dx_string_create("generator", sizeof("generator") - 1);
    if (!name) {
      goto END;
    }
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      goto END;
    }
    generator = dx_adl_node_get_string(child_node);
    if (!generator) {
      goto END;
    }
  }
  // vertexFormat
  DX_VERTEX_FORMAT vertex_format = DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA_AMBIENT_UV;
  {
    dx_string* name = NAME(vertex_format_key);
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (dx_get_error()) {
      goto END;
    }
    if (child_node) {
      if (_read_vertex_format(child_node, state, &vertex_format)) {
        goto END;
      }
    }
  }
  // operations
  {
    dx_error last_error = dx_get_error();
    dx_string* name = NAME(operations_key);
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (child_node) {
      mesh_operations = _read_mesh_operations(child_node, state);
      if (!mesh_operations) {
        goto END;
      }
    } else {
      if (dx_get_error() != DX_NOT_FOUND) {
        goto END;
      }
      dx_set_error(last_error);
    }
  }
  // material
  {
    dx_string* name = NAME(material_key);
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (!child_node) {
      goto END;
    }
    material = _read_material(child_node, state);
    if (!material) {
      goto END;
    }
  }
  mesh = dx_asset_mesh_create(generator, vertex_format, material);
  if (!mesh) {
    goto END;
  }
  if (mesh_operations) {
    for (dx_size i = 0, n = dx_pointer_array_get_size(mesh_operations); i < n; ++i) {
      dx_asset_mesh_operation* operation = DX_ASSET_MESH_OPERATION(dx_pointer_array_get_at(mesh_operations, i));
      if (dx_asset_mesh_operation_apply(operation, mesh)) {
        DX_UNREFERENCE(mesh);
        mesh = NULL;
        goto END;
      }
    }
  }
END:
  if (material) {
    DX_UNREFERENCE(material);
    material = NULL;
  }
  if (mesh_operations) {
    dx_pointer_array_uninitialize(mesh_operations);
    dx_memory_deallocate(mesh_operations);
  }
  return mesh;
}

static dx_object* read(dx_adl_semantical_mesh_reader* self, dx_adl_node* node, dx_adl_semantical_state* state) {
  return DX_OBJECT(_read_mesh(node, state));
}

int dx_adl_semantical_mesh_reader_construct(dx_adl_semantical_mesh_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_mesh_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_adl_node*, dx_adl_semantical_state*))&read;
  DX_OBJECT(self)->type = _type;
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
