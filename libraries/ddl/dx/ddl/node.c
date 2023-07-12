#include "dx/ddl/node.h"

#include <string.h>

DX_DEFINE_OBJECT_TYPE("dx.ddl.node",
                      dx_ddl_node,
                      dx_object)

static void on_added(dx_object** a);

static void on_removed(dx_object** a);

static dx_size on_hash_key(dx_object** a);

static bool on_compare_keys(dx_object** a, dx_object** b);

static void on_added(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_REFERENCE(*a);
}

static void on_removed(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_UNREFERENCE(*a);
}

static dx_size on_hash_key(dx_object** a) {
  return dx_string_get_hash_value(DX_STRING(*a));
}

static bool on_compare_keys(dx_object** a, dx_object** b) {
  return dx_string_is_equal_to(DX_STRING(*a), DX_STRING(*b));
}

int dx_ddl_node_construct(dx_ddl_node* self, dx_ddl_node_kind kind) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_ddl_node_get_type();
  if (!_type) {
    return 1;
  }
  self->kind = kind;
  switch (self->kind) {
    case dx_ddl_node_kind_error: {
      return 0;
    } break;
    case dx_ddl_node_kind_list: {
      if (dx_pointer_array_initialize(&self->list, 0, (void(*)(void**)) & on_added, (void(*)(void**)) &on_removed)) {
        return 1;
      }
    } break;
    case dx_ddl_node_kind_map: {
      static DX_POINTER_HASHMAP_CONFIGURATION const configuration = {
        .compare_keys_callback = (bool(*)(void**,void**)) &on_compare_keys,
        .hash_key_callback = (dx_size(*)(void**)) & on_hash_key,
        .key_added_callback = (void(*)(void**)) &on_added,
        .key_removed_callback = (void(*)(void**)) &on_removed,
        .value_added_callback = (void(*)(void**)) &on_added,
        .value_removed_callback = (void(*)(void**)) &on_removed,
      };
      if (dx_pointer_hashmap_initialize(&self->map, &configuration)) {
        return 1;
      }
    } break;
    case dx_ddl_node_kind_number: {
      self->number = dx_string_create("0", sizeof("0") - 1);
      if (!self->number) {
        return 1;
      }
    } break;
    case dx_ddl_node_kind_string: {
      self->string = dx_string_create("", sizeof("") - 1);
      if (!self->string) {
        return 1;
      }
    } break;
    default: {
      dx_set_error(DX_INVALID_ARGUMENT);
      return 1;
    } break;
  };
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_ddl_node_destruct(dx_ddl_node* self) {
  DX_DEBUG_ASSERT(NULL != self);
  switch (self->kind) {
    case dx_ddl_node_kind_error: {
    } break;
    case dx_ddl_node_kind_list: {
      dx_pointer_array_uninitialize(&self->list);
    } break;
    case dx_ddl_node_kind_map: {
      dx_pointer_hashmap_uninitialize(&self->map);
    } break;
    case dx_ddl_node_kind_number: {
      DX_DEBUG_ASSERT(NULL != self->number);
      DX_DEBUG_ASSERT(NULL != self->string);
      DX_UNREFERENCE(self->number);
      self->number = NULL;
    } break;
    case dx_ddl_node_kind_string: {
      DX_DEBUG_ASSERT(NULL != self->string);
      DX_UNREFERENCE(self->string);
      self->string = NULL;
    } break;
  };
}

dx_ddl_node* dx_ddl_node_create(dx_ddl_node_kind kind) {
  dx_ddl_node* self = DX_DDL_NODE(dx_object_alloc(sizeof(dx_ddl_node)));
  if (!self) {
    return NULL;
  }
  if (dx_ddl_node_construct(self, kind)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

dx_ddl_node_kind dx_ddl_node_get_kind(dx_ddl_node const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return self->kind;
}

int dx_ddl_node_map_set(dx_ddl_node* self, dx_string* name, dx_ddl_node* value) {
  if (!self || !name || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_map) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return dx_pointer_hashmap_set(&self->map, name, value);
}

dx_ddl_node* dx_ddl_node_map_get(dx_ddl_node const* self, dx_string* name) {
  if (!self || !name) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->kind != dx_ddl_node_kind_map) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  return dx_pointer_hashmap_get(&self->map, name);
}

int dx_ddl_node_list_append(dx_ddl_node* self, dx_ddl_node* value) {
  if (!self || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return dx_pointer_array_append(&self->list, (void*)value);
}

int dx_ddl_node_list_prepend(dx_ddl_node* self, dx_ddl_node* value) {
  if (!self || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return dx_pointer_array_prepend(&self->list, (void*)value);
}

int dx_ddl_node_list_insert(dx_ddl_node* self, dx_ddl_node* value, dx_size index) {
  if (!self || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return dx_pointer_array_insert(&self->list, (void*)value, index);
}

dx_ddl_node* dx_ddl_node_list_get(dx_ddl_node* self, dx_size index) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  return dx_pointer_array_get_at(&self->list, index);
}

dx_size dx_ddl_node_list_get_size(dx_ddl_node* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return 0;
  }
  return dx_pointer_array_get_size(&self->list);
}

dx_string* dx_ddl_node_get_string(dx_ddl_node const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->kind != dx_ddl_node_kind_string) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  DX_REFERENCE(self->string);
  return self->string;
}

int dx_ddl_node_set_string(dx_ddl_node* self, dx_string* string) {
  if (!self || !string) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_string) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  DX_REFERENCE(string);
  DX_UNREFERENCE(self->string);
  self->string = string;
  return 0;
}

dx_string* dx_ddl_node_get_number(dx_ddl_node const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->kind != dx_ddl_node_kind_number) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  return self->number;
}

int dx_ddl_node_set_number(dx_ddl_node* self, dx_string* number) {
  if (!self || !number) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_number) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  DX_REFERENCE(number);
  DX_UNREFERENCE(self->number);
  self->number = number;
  return 0;
}
