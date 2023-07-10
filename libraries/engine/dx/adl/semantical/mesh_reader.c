#include "dx/adl/semantical/mesh_reader.h"

#include "dx/asset/mesh.h"
#include "dx/adl/semantical/read.h"
#include "dx/asset/mesh_operation.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}
#define NAME(name) _get_name(context->names, dx_semantical_name_index_##name)

static dx_asset_material* _read_material(dx_ddl_node* node, dx_adl_context* context);

static void on_object_added(dx_object** o);

static void on_object_removed(dx_object** o);

static int _read_mesh_operation(dx_ddl_node* node, dx_adl_context* context, dx_pointer_array* mesh_operations);

static dx_pointer_array* _read_mesh_operations(dx_ddl_node* node, dx_adl_context* context);

static int _read_vertex_format(dx_ddl_node* node, dx_adl_context* context, DX_VERTEX_FORMAT* vertex_format);

static dx_asset_mesh* _read_mesh(dx_ddl_node* node, dx_adl_context* context);

static int complete(dx_adl_semantical_mesh_reader* self,
                    dx_adl_symbol *symbol,
                    dx_adl_context* context);

static dx_object* read(dx_adl_semantical_mesh_reader* self,
                       dx_ddl_node* node,
                       dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.mesh_reader",
                      dx_adl_semantical_mesh_reader,
                      dx_adl_semantical_reader)

static dx_asset_material* _read_material(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_material* asset = NULL;
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(material_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, NAME(material_type));
    if (!reader) {
      return NULL;
    }
    asset = DX_ASSET_MATERIAL(dx_adl_semantical_reader_read(reader, node, context));
    if (!asset) {
      goto END;
    }
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
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

static int _read_mesh_operation(dx_ddl_node* node, dx_adl_context* context, dx_pointer_array* mesh_operations) {
  if (!node) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (node->kind != dx_ddl_node_kind_map) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return 1;
  }
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return 1;
  }
  dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, received_type);
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  if (!reader) {
    if (dx_get_error() != DX_NOT_FOUND) {
      return 1;
    } else {
      dx_set_error(DX_SEMANTICAL_ERROR);
      return 1;
    }
  }
  dx_asset_mesh_operation* mesh_operation = DX_ASSET_MESH_OPERATION(dx_adl_semantical_reader_read(reader, node, context));
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

static dx_pointer_array* _read_mesh_operations(dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (node->kind != dx_ddl_node_kind_list) {
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
  for (dx_size i = 0, n = dx_ddl_node_list_get_size(node); i < n; ++i) {
    dx_ddl_node* child_node = dx_ddl_node_list_get(node, i);
    if (!child_node) {
      dx_pointer_array_uninitialize(mesh_operations);
      dx_memory_deallocate(mesh_operations);
      mesh_operations = NULL;
      return NULL;
    }
    if (_read_mesh_operation(child_node, context, mesh_operations)) {
      dx_pointer_array_uninitialize(mesh_operations);
      dx_memory_deallocate(mesh_operations);
      mesh_operations = NULL;
      return NULL;
    }
  }
  return mesh_operations;
}

static int _read_vertex_format(dx_ddl_node* node, dx_adl_context* context, DX_VERTEX_FORMAT* vertex_format) {
  if (!node) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (node->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return 1;
  }
  DX_VERTEX_FORMAT vertex_format_1 = 0;
  for (dx_size i = 0, n = dx_ddl_node_list_get_size(node); i < n; ++i) {
    dx_ddl_node* child_node = dx_ddl_node_list_get(node, i);
    dx_string* received_value = dx_ddl_node_get_string(child_node);
    if (dx_string_is_equal_to(received_value, NAME(position_xyz_string))) {
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      vertex_format_1 |= DX_VERTEX_FORMAT_POSITION_XYZ;
    } else if (dx_string_is_equal_to(received_value, NAME(ambient_rgba_string))) {
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      vertex_format_1 |= DX_VERTEX_FORMAT_AMBIENT_RGBA;
    } else if (dx_string_is_equal_to(received_value, NAME(ambient_uv_string))) {
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      vertex_format_1 |= DX_VERTEX_FORMAT_AMBIENT_UV;
    } else {
      dx_set_error(DX_SEMANTICAL_ERROR);
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      return 1;
    }
  }
  (*vertex_format) = vertex_format_1;
  return 0;
}

static dx_asset_mesh* _read_mesh(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_mesh* mesh_value = NULL;
  dx_string* name_value = NULL;
  dx_string* generator_value = NULL;
  dx_asset_reference* material_reference_value = NULL;
  dx_pointer_array* operations_value = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, context);
    if (!name_value) {
      goto END;
    }
  }
  // generator
  {
    dx_string* name = dx_string_create("generator", sizeof("generator") - 1);
    if (!name) {
      goto END;
    }
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      goto END;
    }
    generator_value = dx_ddl_node_get_string(child_node);
    if (!generator_value) {
      goto END;
    }
  }
  // vertexFormat
  DX_VERTEX_FORMAT vertex_format_value = DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA_AMBIENT_UV;
  {
    dx_string* name = NAME(vertex_format_key);
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
    if (dx_get_error()) {
      goto END;
    }
    if (child_node) {
      if (_read_vertex_format(child_node, context, &vertex_format_value)) {
        goto END;
      }
    }
  }
  // operations
  {
    dx_error last_error = dx_get_error();
    dx_string* name = NAME(operations_key);
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
    if (child_node) {
      operations_value = _read_mesh_operations(child_node, context);
      if (!operations_value) {
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
    material_reference_value = dx_adl_semantical_read_material_instance_field(node, false, NAME(material_key), context);
    if (!material_reference_value) {
      goto END;
    }
  }
  mesh_value = dx_asset_mesh_create(name_value, generator_value, vertex_format_value, material_reference_value);
  DX_UNREFERENCE(material_reference_value);
  material_reference_value = NULL;
  DX_UNREFERENCE(generator_value);
  generator_value = NULL;
  DX_UNREFERENCE(name_value);
  name_value = NULL;
  if (!mesh_value) {
    goto END;
  }
  if (operations_value) {
    for (dx_size i = 0, n = dx_pointer_array_get_size(operations_value); i < n; ++i) {
      dx_asset_mesh_operation* operation_value = DX_ASSET_MESH_OPERATION(dx_pointer_array_get_at(operations_value, i));
      if (dx_asset_mesh_operation_apply(operation_value, mesh_value)) {
        DX_UNREFERENCE(mesh_value);
        mesh_value = NULL;
        goto END;
      }
    }
  }
END:
  if (name_value) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  }
  if (generator_value) {
    DX_UNREFERENCE(generator_value);
    generator_value = NULL;
  }
  if (material_reference_value) {
    DX_UNREFERENCE(material_reference_value);
    material_reference_value = NULL;
  }
  if (operations_value) {
    dx_pointer_array_uninitialize(operations_value);
    dx_memory_deallocate(operations_value);
  }
  return mesh_value;
}

static int complete(dx_adl_semantical_mesh_reader* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_asset_mesh* mesh = DX_ASSET_MESH(symbol->asset);
  if (mesh->material_reference) {
    if (mesh->material_reference->object) {
      return 0;
    }
    dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, mesh->material_reference->name);
    if (!referenced_symbol) {
      return 1;
    }
    if (!referenced_symbol->asset) {
      return 1;
    }
    mesh->material_reference->object = referenced_symbol->asset;
    DX_REFERENCE(mesh->material_reference->object);
  }
  return 0;
}

static dx_object* read(dx_adl_semantical_mesh_reader* self, dx_ddl_node* node, dx_adl_context* context) {
  return DX_OBJECT(_read_mesh(node, context));
}

int dx_adl_semantical_mesh_reader_construct(dx_adl_semantical_mesh_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_mesh_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->complete = (int(*)(dx_adl_semantical_reader*, dx_adl_symbol*, dx_adl_context*))&complete;
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_ddl_node*, dx_adl_context*))&read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_mesh_reader_destruct(dx_adl_semantical_mesh_reader* self)
{/*Intentionally empty.*/}

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
