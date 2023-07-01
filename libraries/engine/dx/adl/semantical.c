#include "dx/adl/semantical.h"

#include "dx/adl/semantical/read.h"

#include "dx/asset/mesh_instance.h"
#include "dx/asset/viewer_instance.h"
#include "dx/adl/semantical/names.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline dx_string* _get_name(dx_adl_semantical_names* names, size_t index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_object* _read_scene_element(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_asset_scene* _read_scene(dx_adl_node* node, dx_adl_semantical_state* state);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_object* _read_scene_element(dx_adl_node* node, dx_adl_semantical_state* state) {
  if (node->kind != dx_adl_node_kind_map) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
  dx_string* received_type = dx_adl_semantical_read_type(node, state);
  if (!received_type) {
    return NULL;
  }
  if (!dx_string_is_equal_to(received_type, NAME(mesh_instance_type)) &&
      !dx_string_is_equal_to(received_type, NAME(viewer_instance_type))) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return NULL;
  }
  dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&state->readers, received_type);
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  if (!reader) {
    return NULL;
  }
  dx_object* asset = dx_adl_semantical_reader_read(reader, node, state);
  return asset;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_scene* _read_scene(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_scene* scene_value = dx_asset_scene_create();
  if (!scene_value) {
    goto END;
  }
  // !!! store the scene in the state !!!
  state->scene = scene_value;
  DX_REFERENCE(scene_value);
  // elements
  {
    dx_string* name = NAME(elements_key);
    dx_adl_node* node1 = dx_adl_node_map_get(node, name);
    if (!node1) {
      goto END;
    }
    if (node1->kind != dx_adl_node_kind_list) {
      goto END;
    }
    for (dx_size i = 0, n = dx_adl_node_list_get_size(node1); i < n; ++i) {
      dx_adl_node* node2 = dx_adl_node_list_get(node1, i);
      if (!node2) {
        goto END;
      }
      dx_object* scene_element_value = _read_scene_element(node2, state);
      if (!scene_element_value) {
        goto END;
      }
      if (dx_object_array_append(&scene_value->mesh_instances, scene_element_value)) {
        DX_UNREFERENCE(scene_element_value);
        scene_element_value = NULL;
        goto END;
      }
      DX_UNREFERENCE(scene_element_value);
      scene_element_value = NULL;
    }
  }
END:
  return scene_value;
}

dx_asset_scene* dx_adl_parse_scene(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string* received_type = dx_adl_semantical_read_type(node, state);
  if (!received_type) {
    return NULL;
  }
  dx_string* expected_type = NAME(scene_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_asset_scene* asset = _read_scene(node, state);
    return asset;
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
