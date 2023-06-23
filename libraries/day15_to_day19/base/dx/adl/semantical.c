#include "dx/adl/semantical.h"

#include "dx/adl/syntactical.h"
#include "dx/asset/palette.h"
#include "dx/adl/semantical/read.h"
// strlen
#include <string.h>

static inline dx_string* _get_name(dx_adl_semantical_names* names, size_t index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}
#define NAME(name) _get_name((names), dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_string* _parse_type(dx_adl_node* node, dx_adl_semantical_names* names);

static int dx_adl_semantical_read_vertex_format(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names, DX_VERTEX_FORMAT* vertex_format);

static int _parse_color_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names, DX_RGB_U8* target);

/// @param palette A pointer to a palette or the null pointer.
static int _parse_rgb_u8(dx_adl_node* node, char const* name, dx_adl_semantical_state* state, dx_adl_semantical_names* names, DX_RGB_U8* target);

static dx_string* _parse_type(dx_adl_node* node, dx_adl_semantical_names* names) {
  dx_string* key = NAME(type_key);
  dx_adl_node* child_node = dx_adl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_adl_node_kind_string) {
    return NULL;
  }
  dx_string* type = dx_adl_node_get_string(child_node);
  DX_DEBUG_CHECK_MAGIC_BYTES(type);
  return type;
}

static int dx_adl_semantical_read_vertex_format(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names, DX_VERTEX_FORMAT* vertex_format) {
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

static int _parse_color_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names, DX_RGB_U8* target) {
  dx_string* expected_type = NAME(color_instance_type);
  dx_string* received_type = _parse_type(node, names);
  if (!received_type) {
    return 1;
  }
  if (!dx_string_is_equal_to(received_type, expected_type)) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return 1;
  }
  dx_string* value = dx_adl_semantical_read_string(node, NAME(reference_key), names);
  if (!value) {
    return 1;
  }
  dx_asset_palette_entry* palette_entry = dx_asset_palette_get(state->scene->palette, value);
  if (!palette_entry) {
    return 1;
  }
  (*target) = palette_entry->value;
  return 0;
}

static int _parse_rgb_u8(dx_adl_node* node, char const* name, dx_adl_semantical_state* state, dx_adl_semantical_names* names, DX_RGB_U8* target) {
  dx_string* name1 = dx_string_create(name, strlen(name));
  if (!name1) {
    return 1;
  }
  dx_adl_node* child_node = dx_adl_node_map_get(node, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  return _parse_color_instance(child_node, state, names, target);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_texture* dx_adl_parse_texture(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_material* _parse_material(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_material* dx_adl_parse_material(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_mesh* _parse_mesh(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_mesh* dx_adl_parse_mesh(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_mesh_instance* _parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_mesh_instance* dx_adl_parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static dx_asset_scene* _parse_scene(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_texture* dx_adl_parse_texture(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_texture* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  expected_type = NAME(texture_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    dx_adl_semantical_reader* texture_reader = dx_pointer_hashmap_get(&state->readers, NAME(texture_type));
    if (!texture_reader) {
      return NULL;
    }
    asset = DX_ASSET_TEXTURE(dx_adl_semantical_reader_read(texture_reader, node, state, names));
    if (!asset) {
      return NULL;
    }
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_material* _parse_material(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_material* material = NULL;
  dx_asset_material* material1 = dx_asset_material_create();
  if (!material1) {
    goto END;
  }
  // ambientColor?
  {
    dx_string* name = NAME(ambient_color_key);
    dx_error old_error = dx_get_error();
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (child_node) {
      DX_RGB_U8 ambient_color_u8;
      if (_parse_color_instance(child_node, state, names, &ambient_color_u8)) {
        goto END;
      }
      DX_VEC4 ambient_color_f32;
      ambient_color_f32.e[0] = ((dx_f32)ambient_color_u8.r) / 255.f;
      ambient_color_f32.e[1] = ((dx_f32)ambient_color_u8.g) / 255.f;
      ambient_color_f32.e[2] = ((dx_f32)ambient_color_u8.b) / 255.f;
      ambient_color_f32.e[3] = 1.f;
      if (dx_asset_material_set_ambient_color(material1, &ambient_color_f32)) {
        goto END;
      }
    } else {
      dx_set_error(old_error);
    }
  }
  // ambientTexture?
  {
    dx_string* name = NAME(ambient_texture_key);
    dx_error old_error = dx_get_error();
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (child_node) {
      dx_asset_texture* ambient_texture = dx_adl_parse_texture(child_node, state, names);
      if (!ambient_texture) {
        goto END;
      }
      if (dx_asset_material_set_ambient_texture(material1, ambient_texture)) {
        DX_UNREFERENCE(ambient_texture);
        ambient_texture = NULL;
        goto END;
      }
      DX_UNREFERENCE(ambient_texture);
      ambient_texture = NULL;
    } else {
      dx_set_error(old_error);
    }
  }
  material = material1;
END:
  return material;
}

static dx_asset_material* dx_adl_parse_material(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_material* asset = NULL;
  dx_string* received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(material_type))) {
    asset = _parse_material(node, state, names);
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void on_object_added(dx_object** o) {
  DX_REFERENCE(*o);
}

static void on_object_removed(dx_object** o) {
  DX_UNREFERENCE(*o);
}

#include "dx/asset/mesh_operation.h"

int _read_mesh_operation(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names, dx_pointer_array* mesh_operations) {
  if (node->kind != dx_adl_node_kind_map) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return 1;
  }
  dx_string* received_type = _parse_type(node, names);
  if (!received_type) {
    return 1;
  }
  dx_asset_mesh_operation* mesh_operation = dx_pointer_hashmap_get(&state->scene->mesh_operations, received_type);
  if (!mesh_operation) {
    if (dx_get_error() != DX_NOT_FOUND) {
      return 1;
    } else {
      dx_set_error(DX_SEMANTICAL_ERROR);
      return 1;
    }
  }
  return 0;
}

dx_pointer_array* read_mesh_operations(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
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
    if (_read_mesh_operation(child_node, state, names, mesh_operations)) {
      dx_pointer_array_uninitialize(mesh_operations);
      dx_memory_deallocate(mesh_operations);
      mesh_operations = NULL;
      return NULL;
    }
  }
  return mesh_operations;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_mesh* _parse_mesh(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
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
      if (dx_adl_semantical_read_vertex_format(child_node, state, names, &vertex_format)) {
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
      mesh_operations = read_mesh_operations(child_node, state, names);
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
    material = dx_adl_parse_material(child_node, state, names);
    if (!material) {
      goto END;
    }
  }
  mesh = dx_asset_mesh_create(generator, vertex_format, material);
  if (!mesh) {
    goto END;
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

static dx_asset_mesh* dx_adl_parse_mesh(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_mesh* asset = NULL;
  dx_string* received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(mesh_type))) {
    asset = _parse_mesh(node, state, names);
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_mesh_instance* _parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
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
    mesh = _parse_mesh(child_node, state, names);
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
      transformation = dx_adl_parse_translation(child_node, state, names);
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
  return mesh_instance;
}

static dx_asset_mesh_instance* dx_adl_parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_mesh_instance* asset = NULL;
  dx_string* received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(mesh_instance_type))) {
    asset = _parse_mesh_instance(node, state, names);
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_scene* _parse_scene(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_scene* scene = dx_asset_scene_create();
  if (!scene) {
    goto END;
  }
  // !!! store the scene in the state !!!
  state->scene = scene;
  DX_REFERENCE(state->scene);
  // meshInstances
  {
    dx_string* name = dx_string_create("meshInstances", sizeof("meshInstances") - 1);
    if (!name) {
      DX_UNREFERENCE(scene);
      scene = NULL;
      goto END;
    }
    dx_adl_node* node1 = dx_adl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!node1 || node1->kind != dx_adl_node_kind_list) {
      DX_UNREFERENCE(scene);
      scene = NULL;
      goto END;
    }
    for (size_t i = 0, n = dx_adl_node_list_get_size(node1); i < n; ++i) {
      dx_adl_node* node2 = dx_adl_node_list_get(node1, i);
      if (!node2) {
        DX_UNREFERENCE(scene);
        scene = NULL;
        goto END;
      }
      dx_asset_mesh_instance* mesh_instance = _parse_mesh_instance(node2, state, names);
      if (!mesh_instance) {
        DX_UNREFERENCE(scene);
        scene = NULL;
        goto END;
      }
      if (dx_object_array_append(&scene->mesh_instances, DX_OBJECT(mesh_instance))) {
        DX_UNREFERENCE(mesh_instance);
        mesh_instance = NULL;
        DX_UNREFERENCE(scene);
        scene = NULL;
        goto END;
      }
      DX_UNREFERENCE(mesh_instance);
      mesh_instance = NULL;
    }
  }
END:
  return scene;
}

dx_asset_scene* dx_adl_parse_scene(dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  dx_asset_scene* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = _parse_type(node, names);
  if (!received_type) {
    return NULL;
  }
  expected_type = NAME(scene_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    asset = _parse_scene(node, state, names);
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
