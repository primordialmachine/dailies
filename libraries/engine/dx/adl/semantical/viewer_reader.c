#include "dx/adl/semantical/viewer_reader.h"

#include "dx/asset/optics.h"
#include "dx/asset/viewer.h"
#include "dx/adl/semantical/read.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int resolve(dx_adl_semantical_viewer_reader* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_semantical_viewer_reader* self, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.viewer_reader",
                      dx_adl_semantical_viewer_reader,
                      dx_adl_semantical_reader)

static dx_asset_optics* _read_optics(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(optics_orthographic_type)) ||
      dx_string_is_equal_to(received_type, NAME(optics_perspective_type))) {
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, received_type);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    if (!reader) {
      return NULL;
    }
    return DX_ASSET_OPTICS(dx_adl_semantical_reader_read(reader, node, context));
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
}

static int resolve(dx_adl_semantical_viewer_reader* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  symbol->resolved = true;
  return 0;
}

static dx_object* read(dx_adl_semantical_viewer_reader* self, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_viewer* viewer_value = NULL;
  dx_string* name_value = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, context);
    if (!name_value) {
      return NULL;
    }
  }
  viewer_value = dx_asset_viewer_create(name_value);
  DX_UNREFERENCE(name_value);
  name_value = NULL;
  if (!viewer_value) {
    return NULL;
  }
  // source?
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(source_key));
    if (!child_node) {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
    } else {
      DX_VEC3* value = dx_adl_semantical_read_vector_3(child_node, context);
      if (!value) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
      viewer_value->source = *value;
      dx_memory_deallocate(value);
      value = NULL;
    }
  }
  // target?
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(target_key));
    if (!child_node) {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
    } else {
      DX_VEC3* value = dx_adl_semantical_read_vector_3(child_node, context);
      if (!value) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
      viewer_value->target = *value;
      dx_memory_deallocate(value);
      value = NULL;
    }
  }
  // up?
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(up_key));
    if (!child_node) {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
    } else {
      DX_VEC3* value = dx_adl_semantical_read_vector_3(child_node, context);
      if (!value) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
      viewer_value->up = *value;
      dx_memory_deallocate(value);
      value = NULL;
    }
  }
  // optics
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(optics_key));
    if (child_node) {
      dx_asset_optics* optics = _read_optics(child_node, context);
      if (!optics) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
      if (viewer_value->optics) {
        DX_UNREFERENCE(viewer_value->optics);
        viewer_value->optics = NULL;
      }
      viewer_value->optics = optics;
    } else {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
    }
  }
  return DX_OBJECT(viewer_value);
}

int dx_adl_semantical_viewer_reader_construct(dx_adl_semantical_viewer_reader* self) {
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  dx_rti_type* _type = dx_adl_semantical_viewer_reader_get_type();
  if (!_type) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->resolve = (int(*)(dx_adl_semantical_reader*, dx_adl_symbol*, dx_adl_context*)) & resolve;
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_ddl_node*, dx_adl_context*))&read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_viewer_reader_destruct(dx_adl_semantical_viewer_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_viewer_reader* dx_adl_semantical_viewer_reader_create() {
  dx_adl_semantical_viewer_reader* self = DX_ADL_SEMANTICAL_VIEWER_READER(dx_object_alloc(sizeof(dx_adl_semantical_viewer_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_viewer_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
