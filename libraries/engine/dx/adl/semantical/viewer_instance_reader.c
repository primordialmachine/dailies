#include "dx/adl/semantical/viewer_instance_reader.h"

#include "dx/asset/viewer_instance.h"
#include "dx/adl/semantical/read.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_semantical_name_index_##name)

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.viewer_instance_reader",
                      dx_adl_semantical_viewer_instance_reader,
                      dx_adl_semantical_reader)

static dx_asset_viewer* _read_viewer(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(viewer_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, NAME(viewer_type));
    if (!reader) {
      return NULL;
    }
    return  DX_ASSET_VIEWER(dx_adl_semantical_reader_read(reader, node, context));
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
}

static dx_asset_viewer_instance* _read_viewer_instance(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_viewer* viewer_value = NULL;
  // viewer
  {
    dx_string* name = NAME(viewer_key);
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
    if (!child_node) {
      return NULL;
    }
    if (child_node->kind != dx_ddl_node_kind_map) {
      return NULL;
    }
    dx_string* received_type = dx_adl_semantical_read_type(child_node, context);
    if (!dx_string_is_equal_to(received_type, NAME(viewer_type))) {
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return NULL;
    }
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, received_type);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    if (!reader) {
      return NULL;
    }
    viewer_value = (dx_asset_viewer*)dx_adl_semantical_reader_read(reader, child_node, context);
    if (!viewer_value) {
      return NULL;
    }
  }
  dx_asset_viewer_instance* viewer_instance_value = dx_asset_viewer_instance_create(viewer_value);
  DX_UNREFERENCE(viewer_value);
  viewer_value = NULL;
  return viewer_instance_value;
}

static dx_object* read(dx_adl_semantical_viewer_instance_reader* self, dx_ddl_node* node, dx_adl_context* context) {
  return DX_OBJECT(_read_viewer_instance(node, context));
}

int dx_adl_semantical_viewer_instance_reader_construct(dx_adl_semantical_viewer_instance_reader* self) {
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  dx_rti_type* _type = dx_adl_semantical_viewer_instance_reader_get_type();
  if (!_type) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object * (*)(dx_adl_semantical_reader*, dx_ddl_node*, dx_adl_context*)) & read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_viewer_instance_reader_destruct(dx_adl_semantical_viewer_instance_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_viewer_instance_reader* dx_adl_semantical_viewer_instance_reader_create() {
  dx_adl_semantical_viewer_instance_reader* self = DX_ADL_SEMANTICAL_VIEWER_INSTANCE_READER(dx_object_alloc(sizeof(dx_adl_semantical_viewer_instance_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_viewer_instance_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
