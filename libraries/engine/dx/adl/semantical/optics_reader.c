#include "dx/adl/semantical/optics_reader.h"

#include "dx/asset/optics.h"
#include "dx/adl/semantical/read.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}
#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

static dx_asset_optics* _read_optics(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_object* read(dx_adl_semantical_optics_reader*,
                       dx_adl_node* node,
                       dx_adl_semantical_state*);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.optics_reader",
                      dx_adl_semantical_optics_reader,
                      dx_adl_semantical_reader)

static dx_asset_optics_perspective* _read_optics_perspective(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_optics_perspective* optics_value = dx_asset_optics_perspective_create();
  if (!optics_value) {
    return NULL;
  }
  // fieldOfViewY?
  {
    dx_f32 value;
    dx_string* key = NAME(field_of_view_y_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->near = value;
    } else {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(optics_value);
        optics_value = NULL;
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  // aspectRatio?
  {
    dx_f32 value;
    dx_string* key = NAME(aspect_ratio_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      if (optics_value->aspect_ratio) {
        optics_value->aspect_ratio = dx_memory_allocate(sizeof(dx_f32));
        if (!optics_value->aspect_ratio) {
          DX_UNREFERENCE(optics_value);
          optics_value = NULL;
          return NULL;
        }
      }
      *optics_value->aspect_ratio = value;
    } else {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(optics_value);
        optics_value = NULL;
        return NULL;
      }
      if (optics_value->aspect_ratio) {
        dx_memory_deallocate(optics_value->aspect_ratio);
        optics_value->aspect_ratio = NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  // near?
  {
    dx_f32 value;
    dx_string* key = NAME(near_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->near = value;
    } else {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(optics_value);
        optics_value = NULL;
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  // far?
  {
    dx_f32 value;
    dx_string* key = NAME(far_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->far = value;
    } else {
      if (dx_get_error() != DX_NOT_FOUND) {
        DX_UNREFERENCE(optics_value);
        optics_value = NULL;
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  return optics_value;
}

static dx_asset_optics_orthographic* _read_optics_orthographic(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_optics_orthographic* optics_value = dx_asset_optics_orthographic_create();
  if (!optics_value) {
    return NULL;
  }
  // near?
  {
    dx_f32 value;
    dx_string* key = NAME(near_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->near = value;
    } else {
      if (dx_get_error() != DX_NOT_FOUND) {
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  // far?
  {
    dx_f32 value;
    dx_string* key = NAME(far_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->far = value;
    } else {
      if (dx_get_error() != DX_NOT_FOUND) {
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  return optics_value;
}

static dx_asset_optics* _read_optics(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string* type = dx_adl_semantical_read_type(node, state);
  if (!type) {
    return NULL;
  }
  if (dx_string_is_equal_to(type, NAME(optics_perspective_type))) {
    DX_UNREFERENCE(type);
    type = NULL;
    return DX_ASSET_OPTICS(_read_optics_perspective(node, state));
  } else if (dx_string_is_equal_to(type, NAME(optics_orthographic_type))) {
    DX_UNREFERENCE(type);
    type = NULL;
    return DX_ASSET_OPTICS(_read_optics_orthographic(node, state));
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    DX_UNREFERENCE(type);
    type = NULL;
    return NULL;
  }
  return NULL;
}

static dx_object* read(dx_adl_semantical_optics_reader* self, dx_adl_node* node, dx_adl_semantical_state* state) {
  return DX_OBJECT(_read_optics(node, state));
}

int dx_adl_semantical_optics_reader_construct(dx_adl_semantical_optics_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_optics_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_adl_node*, dx_adl_semantical_state*))&read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_optics_reader_destruct(dx_adl_semantical_optics_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_optics_reader* dx_adl_semantical_optics_reader_create() {
  dx_adl_semantical_optics_reader* self = DX_ADL_SEMANTICAL_OPTICS_READER(dx_object_alloc(sizeof(dx_adl_semantical_optics_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_optics_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
