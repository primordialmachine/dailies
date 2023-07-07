#include "dx/adl.h"

#include "dx/adl/semantical/read.h"

#include "dx/asset/mesh_instance.h"
#include "dx/asset/viewer_instance.h"
#include "dx/adl/semantical/names.h"
#include "dx/adl/enter.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_object* _read_scene_element(dx_ddl_node* node, dx_adl_context* context);

static int _read_scene(dx_ddl_node* node, dx_adl_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_object* _read_scene_element(dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (node->kind != dx_ddl_node_kind_map) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(mesh_instance_type)) ||
      dx_string_is_equal_to(received_type, NAME(viewer_instance_type))) {
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, received_type);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    if (!reader) {
      return NULL;
    }
    dx_object* asset = dx_adl_semantical_reader_read(reader, node, context);
    return asset;
  } else {
    dx_log("ignoring scene element `", sizeof("ignoring scene element `") - 1);
    dx_log(received_type->bytes, received_type->number_of_bytes);
    dx_log("`", sizeof("`") - 1);
    dx_log("\n", sizeof("\n") - 1);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int _read_scene(dx_ddl_node* node, dx_adl_context* context) {
  // elements
  {
    dx_string* name = NAME(elements_key);
    dx_ddl_node* node1 = dx_ddl_node_map_get(node, name);
    if (!node1) {
      return 1;
    }
    if (node1->kind != dx_ddl_node_kind_list) {
      return 1;
    }
    for (dx_size i = 0, n = dx_ddl_node_list_get_size(node1); i < n; ++i) {
      dx_ddl_node* node2 = dx_ddl_node_list_get(node1, i);
      if (!node2) {
        return 1;
      }
      dx_object* scene_element_value = _read_scene_element(node2, context);
      if (scene_element_value) {
        if (dx_object_array_append(&context->scene->mesh_instances, scene_element_value)) {
          DX_UNREFERENCE(scene_element_value);
          scene_element_value = NULL;
          return 1;
        }
        DX_UNREFERENCE(scene_element_value);
        scene_element_value = NULL;
      } else {
        if (dx_get_error()) {
          return 1;
        }
      }
    }
  }
  return 0;
}

static int dx_adl_compiler_enter_phase(dx_ddl_node* node, dx_adl_context* context) {
  dx_adl_enter* enter = dx_adl_enter_create(context);
  if (!enter) {
    return 1;
  }
  if (dx_adl_enter_run(enter, node, context)) {
    DX_UNREFERENCE(enter);
    enter = NULL;
    return 1;
  }
  DX_UNREFERENCE(enter);
  enter = NULL;
  return 0;
}

static int dx_adl_compiler_complete_phase(dx_adl_context* context) {
  dx_pointer_hashmap_iterator iterator;
  dx_pointer_hashmap_iterator_initialize(&iterator, &context->definitions->map);
  while (dx_pointer_hashmap_iterator_has_entry(&iterator)) {
    dx_adl_symbol* symbol = dx_pointer_hashmap_iterator_get_value(&iterator);
    if (!symbol->asset) {
      if (dx_string_is_equal_to(symbol->type, NAME(color_type))) {
        dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, symbol->type);
        if (!reader) {
          return 0;
        }
        symbol->asset = dx_adl_semantical_reader_read(reader, symbol->node, context);
      }
    }
    dx_pointer_hashmap_iterator_next(&iterator);
  }
  return 0;
}

dx_asset_scene* dx_adl_compile(dx_ddl_node* node) {
  dx_adl_context* context = dx_adl_context_create();
  if (!context) {
    return NULL;
  }
  dx_asset_scene* scene = dx_asset_scene_create();
  if (!scene) {
    DX_UNREFERENCE(context);
    context = NULL;
    return NULL;
  }
  context->scene = scene;
  if (dx_adl_compiler_enter_phase(node, context)) {
    DX_UNREFERENCE(context);
    context = NULL;
    return NULL;
  }
  if (dx_adl_compiler_complete_phase(context)) {
    DX_UNREFERENCE(context);
    context = NULL;
    return NULL;
  }
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    DX_UNREFERENCE(context);
    context = NULL;
    return NULL;
  }
  dx_string* expected_type = NAME(scene_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    if (_read_scene(node, context)) {
      DX_UNREFERENCE(context);
      context = NULL;
      return NULL;
    }
    DX_REFERENCE(scene);
    DX_UNREFERENCE(context);
    context = NULL;
    return scene;
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    DX_UNREFERENCE(context);
    context = NULL;
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/