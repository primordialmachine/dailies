#include "dx/adl/semantical/state.h"

#include "dx/adl/syntactical.h"

#include "dx/adl/semantical/image_reader.h"
#include "dx/adl/semantical/material_reader.h"
#include "dx/adl/semantical/mesh_instance_reader.h"
#include "dx/adl/semantical/mesh_operations_reader.h"
#include "dx/adl/semantical/mesh_reader.h"
#include "dx/adl/semantical/texture_reader.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_reader",
                      dx_adl_semantical_reader,
                      dx_object)

int dx_adl_semantical_reader_construct(dx_adl_semantical_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_reader_get_type();
  if (!_type) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_reader_destruct(dx_adl_semantical_reader* self)
{/*Intentionally empty.*/}

dx_object* dx_adl_semantical_reader_read(dx_adl_semantical_reader* self,
                                         dx_adl_node* node,
                                         dx_adl_semantical_state* state)
{ return self->read(self, node, state); }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_bool compare_keys_callback(dx_string** key1, dx_string** key2);

static dx_size hash_key_callback(dx_string** key);

static void key_added_callback(dx_string** key);

static void key_removed_callback(dx_string** key);

static void value_added_callback(dx_object** value);

static void value_removed_callback(dx_object** value);

static bool compare_keys_callback(dx_string** key1, dx_string** key2) {
  DX_DEBUG_ASSERT(NULL != key1);
  DX_DEBUG_ASSERT(NULL != *key1);
  DX_DEBUG_ASSERT(NULL != key2);
  DX_DEBUG_ASSERT(NULL != *key2);
  return dx_string_is_equal_to(*key1, *key2);
}

static size_t hash_key_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  return dx_string_get_hash_value(*key);
}

static void key_added_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_REFERENCE(*key);
}

static void key_removed_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_UNREFERENCE(*key);
}

static void value_added_callback(dx_object** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_REFERENCE(*value);
}

static void value_removed_callback(dx_object** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_UNREFERENCE(*value);
}

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_state",
                       dx_adl_semantical_state,
                       dx_object)

static inline dx_string* _get_name(dx_adl_semantical_names* names, size_t index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name((self->names), dx_semantical_name_index_##name)

int dx_adl_semantical_state_construct(dx_adl_semantical_state* self) {
  dx_rti_type* _type = dx_adl_semantical_state_get_type();
  if (!_type) {
    return 1;
  }
  self->names = dx_adl_semantical_names_create();
  if (!self->names) {
    return 1;
  }
  self->scene = NULL;
  {
    static DX_POINTER_HASHMAP_CONFIGURATION const configuration = {
      .compare_keys_callback = (dx_compare_keys_callback*)&compare_keys_callback,
      .hash_key_callback = (dx_hash_key_callback*)&hash_key_callback,
      .key_added_callback = (dx_key_added_callback*)&key_added_callback,
      .key_removed_callback = (dx_key_removed_callback*)&key_removed_callback,
      .value_added_callback = (dx_value_added_callback*)&value_added_callback,
      .value_removed_callback = (dx_value_removed_callback*)&value_removed_callback,
    };
    if (dx_pointer_hashmap_initialize(&self->assets, &configuration)) {
      DX_UNREFERENCE(self->names);
      self->names = NULL;
      return 1;
    }
  }
  {
    static DX_POINTER_HASHMAP_CONFIGURATION const configuration = {
      .compare_keys_callback = (dx_compare_keys_callback*)&compare_keys_callback,
      .hash_key_callback = (dx_hash_key_callback*)&hash_key_callback,
      .key_added_callback = (dx_key_added_callback*)&key_added_callback,
      .key_removed_callback = (dx_key_removed_callback*)&key_removed_callback,
      .value_added_callback = (dx_value_added_callback*)&value_added_callback,
      .value_removed_callback = (dx_value_removed_callback*)&value_removed_callback,
    };
    if (dx_pointer_hashmap_initialize(&self->readers, &configuration)) {
      dx_pointer_hashmap_uninitialize(&self->assets);
      DX_UNREFERENCE(self->names);
      self->names = NULL;
      return 1;
    }
    #define DEFINE2(NAME, TYPE) \
    { \
      dx_string* k = _get_name((self->names), dx_semantical_name_index_##NAME##_type); \
      dx_adl_semantical_reader* v = (dx_adl_semantical_reader*)TYPE##_create(); \
      if (!v) { \
        \
          dx_pointer_hashmap_uninitialize(&self->readers);  \
          dx_pointer_hashmap_uninitialize(&self->assets); \
          DX_UNREFERENCE(self->names); \
          self->names = NULL; \
          return 1; \
      } \
        if (dx_pointer_hashmap_set(&self->readers, k, v)) { \
          \
            DX_UNREFERENCE(v); \
            v = NULL; \
            dx_pointer_hashmap_uninitialize(&self->readers); \
            dx_pointer_hashmap_uninitialize(&self->assets); \
            DX_UNREFERENCE(self->names); \
            self->names = NULL; \
            return 1; \
        } \
        DX_UNREFERENCE(v); \
        v = NULL; \
    }

    #define DEFINE(NAME) \
    { \
      dx_string* k = _get_name((self->names), dx_semantical_name_index_##NAME##_type); \
      dx_adl_semantical_reader* v = (dx_adl_semantical_reader*)dx_adl_semantical_##NAME##_reader_create(); \
      if (!v) { \
        dx_pointer_hashmap_uninitialize(&self->readers); \
        dx_pointer_hashmap_uninitialize(&self->assets); \
        DX_UNREFERENCE(self->names); \
        self->names = NULL; \
        return 1; \
      } \
      if (dx_pointer_hashmap_set(&self->readers, k, v)) { \
        DX_UNREFERENCE(v); \
        v = NULL; \
        dx_pointer_hashmap_uninitialize(&self->readers); \
        dx_pointer_hashmap_uninitialize(&self->assets); \
        DX_UNREFERENCE(self->names); \
        self->names = NULL; \
        return 1; \
      } \
      DX_UNREFERENCE(v); \
      v = NULL; \
    }

    DEFINE(image)
    DEFINE(material)
    DEFINE(mesh)
    DEFINE(mesh_instance)
    DEFINE(texture)
    DEFINE2(mesh_operations_set_vertex_colors, dx_adl_semantical_mesh_operations_reader)

    #undef DEFINE
    #undef DEFINE2
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_state_destruct(dx_adl_semantical_state* self) {
  if (self->scene) {
    DX_UNREFERENCE(self->scene);
    self->scene = NULL;
  }
  dx_pointer_hashmap_uninitialize(&self->readers);
  dx_pointer_hashmap_uninitialize(&self->assets);
  if (self->names) {
    DX_UNREFERENCE(self->names);
    self->names = NULL;
  }
}

dx_adl_semantical_state* dx_adl_semantical_state_create() {
  dx_rti_type* type = dx_adl_semantical_state_get_type();
  if (!type) {
    return NULL;
  }
  dx_adl_semantical_state* self = DX_ADL_SEMANTICAL_STATE(dx_object_alloc(sizeof(dx_adl_semantical_state)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_state_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_adl_semantical_add_reader(dx_adl_semantical_state* self, dx_string* name, dx_adl_semantical_reader* reader) {
  if (!self || !name || !reader) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (dx_pointer_hashmap_set(&self->readers, (dx_pointer_hashmap_key)name, (dx_pointer_hashmap_value)reader)) {
    return 1;
  }
  return 0;
}
