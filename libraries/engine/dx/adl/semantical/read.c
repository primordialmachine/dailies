#include "dx/adl/semantical/read.h"

#include "dx/adl/semantical/state.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, size_t index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}
#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

static int _read_translation(DX_MAT4* target, dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static int _read_vector_3(DX_VEC3* target, dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names);

static int _read_translation(DX_MAT4* target, dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  if (!target || !node || !state) {
    return 1;
  }
  dx_f32 x, y, z;
  if (dx_adl_semantical_read_f32(node, NAME(x_key), &x)) {
    return 1;
  }
  if (dx_adl_semantical_read_f32(node, NAME(y_key), &y)) {
    return 1;
  }
  if (dx_adl_semantical_read_f32(node, NAME(z_key), &z)) {
    return 1;
  }
  dx_mat4_set_translate(target, x, y, z);
  return 0;
}

static int _read_vector_3(DX_VEC3* target, dx_adl_node* node, dx_adl_semantical_state* state, dx_adl_semantical_names* names) {
  if (!target || !node || !state) {
    return 1;
  }
  dx_f32 x, y, z;
  if (dx_adl_semantical_read_f32(node, NAME(x_key), &x)) {
    return 1;
  }
  if (dx_adl_semantical_read_f32(node, NAME(y_key), &y)) {
    return 1;
  }
  if (dx_adl_semantical_read_f32(node, NAME(z_key), &z)) {
    return 1;
  }
  dx_vec3_set(target, x, y, z);
  return 0;
}

DX_MAT4* dx_adl_semantical_read_translation(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string* received_type = dx_adl_semantical_read_type(node, state);
  if (!received_type) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
  if (!dx_string_is_equal_to(received_type, NAME(translation_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    if (!dx_get_error()) {
      dx_set_error(DX_SEMANTICAL_ERROR);
    }
    return NULL;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  DX_MAT4* target = dx_memory_allocate(sizeof(DX_MAT4));
  if (!target) {
    return NULL;
  }
  dx_memory_zero(target, sizeof(DX_MAT4));
  if (_read_translation(target, node, state, state->names)) {
    dx_memory_deallocate(target);
    target = NULL;
    return NULL;
  }
  return target;
}

DX_VEC3* dx_adl_semantical_read_vector_3(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string* received_type = dx_adl_semantical_read_type(node, state);
  if (!received_type) {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
  if (!dx_string_is_equal_to(received_type, NAME(vector3_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    if (!dx_get_error()) {
      dx_set_error(DX_SEMANTICAL_ERROR);
    }
    return NULL;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  DX_VEC3* target = dx_memory_allocate(sizeof(DX_VEC3));
  if (!target) {
    return NULL;
  }
  dx_memory_zero(target, sizeof(DX_VEC3));
  if (_read_vector_3(target, node, state, state->names)) {
    dx_memory_deallocate(target);
    target = NULL;
    return NULL;
  }
  return target;
}

dx_string* dx_adl_semantical_read_type(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string* key = NAME(type_key);
  dx_adl_node* child_node = dx_adl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_adl_node_kind_string) {
    return NULL;
  }
  dx_string* type = dx_adl_node_get_string(child_node);
  DX_DEBUG_CHECK_MAGIC_BYTES(type);
  return type;
}

dx_string* dx_adl_semantical_read_name(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string* key = NAME(name_key);
  dx_adl_node* child_node = dx_adl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_adl_node_kind_string) {
    return NULL;
  }
  dx_string* type = dx_adl_node_get_string(child_node);
  DX_DEBUG_CHECK_MAGIC_BYTES(type);
  return type;
}

int dx_adl_semantical_read_n8(dx_adl_node* node, dx_string* name, dx_n8* target) {
  dx_adl_node* child_node = dx_adl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_adl_node_kind_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_n8(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

int dx_adl_semantical_read_sz(dx_adl_node* node, dx_string* name, dx_size* target) {
  dx_adl_node* child_node = dx_adl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_adl_node_kind_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_sz(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

int dx_adl_semantical_read_f32(dx_adl_node* node, dx_string* name, dx_f32* target) {
  dx_adl_node* child_node = dx_adl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_adl_node_kind_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_f32(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

int dx_adl_semantical_read_f64(dx_adl_node* node, dx_string* name, dx_f64* target) {
  dx_adl_node* child_node = dx_adl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_adl_node_kind_number) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_f64(child_node->number->bytes, child_node->number->number_of_bytes, target)) {
    return 1;
  }
  return 0;
}

dx_string* dx_adl_semantical_read_string(dx_adl_node* node, dx_string* name, dx_adl_semantical_names* names) {
  dx_adl_node* child_node = dx_adl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_adl_node_kind_string) {
    return NULL;
  }
  dx_string* value = dx_adl_node_get_string(child_node);
  if (!value) {
    return NULL;
  }
  DX_DEBUG_CHECK_MAGIC_BYTES(value);
  return value;
}
