#include "dx/adl/semantical/read.h"

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
