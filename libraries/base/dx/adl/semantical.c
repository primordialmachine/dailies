#include "dx/adl/semantical.h"

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

#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_mesh_instance* dx_adl_parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_asset_scene* _parse_scene(dx_adl_node* node, dx_adl_semantical_state* state);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_mesh_instance* dx_adl_parse_mesh_instance(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string* received_type = dx_adl_semantical_read_type(node, state->names);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(mesh_instance_type))) {
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&state->readers, NAME(mesh_instance_type));
    if (!reader) {
      return NULL;
    }
    dx_asset_mesh_instance* asset = DX_ASSET_MESH_INSTANCE(dx_adl_semantical_reader_read(reader, node, state));
    return asset;
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_scene* _parse_scene(dx_adl_node* node, dx_adl_semantical_state* state) {
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
    for (dx_size i = 0, n = dx_adl_node_list_get_size(node1); i < n; ++i) {
      dx_adl_node* node2 = dx_adl_node_list_get(node1, i);
      if (!node2) {
        DX_UNREFERENCE(scene);
        scene = NULL;
        goto END;
      }
      dx_asset_mesh_instance* mesh_instance = dx_adl_parse_mesh_instance(node2, state);
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

dx_asset_scene* dx_adl_parse_scene(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string* received_type = dx_adl_semantical_read_type(node, state->names);
  if (!received_type) {
    return NULL;
  }
  dx_string* expected_type = NAME(scene_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    dx_asset_scene* asset = _parse_scene(node, state);
    return asset;
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
