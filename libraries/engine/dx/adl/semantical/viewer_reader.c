#include "dx/adl/semantical/viewer_reader.h"

#include "dx/asset/viewer.h"
#include "dx/adl/semantical/read.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, size_t index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_object* read(dx_adl_semantical_viewer_reader*,
                       dx_adl_node* node,
                       dx_adl_semantical_state*);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_viewer_reader",
                      dx_adl_semantical_viewer_reader,
                      dx_adl_semantical_reader)

static dx_object* read(dx_adl_semantical_viewer_reader* self, dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_viewer* viewer_value = NULL;
  dx_string* name_value = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, state);
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
    dx_adl_node* child_node = dx_adl_node_map_get(node, NAME(source_key));
    if (!child_node) {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
    } else {
      DX_VEC3* value = dx_adl_semantical_read_vector_3(child_node, state);
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
    dx_adl_node* child_node = dx_adl_node_map_get(node, NAME(target_key));
    if (!child_node) {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
    } else {
      DX_VEC3* value = dx_adl_semantical_read_vector_3(child_node, state);
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
    dx_adl_node* child_node = dx_adl_node_map_get(node, NAME(up_key));
    if (!child_node) {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return NULL;
      }
    } else {
      DX_VEC3* value = dx_adl_semantical_read_vector_3(child_node, state);
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
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_adl_node*, dx_adl_semantical_state*))&read;
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
