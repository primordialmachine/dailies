#include "dx/core/object.h"

#include "dx/core/memory.h"
#include "dx/core/pointer_hashmap.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct _dx_rti_type_name _dx_rti_type_name;

struct _dx_rti_type_name {
  dx_reference_counter reference_count;
  dx_size hash_value;
  dx_size number_of_bytes;
  char bytes[];
};

static _dx_rti_type_name* _dx_rti_type_name_create(char const* bytes, dx_size number_of_bytes) {
  _dx_rti_type_name* self = dx_memory_allocate(sizeof(_dx_rti_type_name) + number_of_bytes);
  if (!self) {
    return NULL;
  }
  self->hash_value = dx_hash_bytes(bytes, number_of_bytes);
  dx_memory_copy(self->bytes, bytes, number_of_bytes);
  self->number_of_bytes = number_of_bytes;
  self->reference_count = 1;
  return self;
}


#define _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL (1)
#define _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION (2)
#define _DX_RTI_TYPE_NODE_FLAGS_OBJECT (4)

typedef struct _dx_rti_type _dx_rti_type;
static inline _dx_rti_type* _DX_RTI_TYPE(void* p) {
  return (_dx_rti_type*)p;
}

struct _dx_rti_type {
  dx_reference_counter reference_count;
  dx_n8 flags;
  _dx_rti_type_name* name;
  void (*on_type_destroyed)();
  union {
    struct {
      dx_size value_size; // _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL
    } fundamental;
    struct {
      _dx_rti_type* parent;
      dx_size value_size;
      void (*destruct)(void*);
    } object;
  };
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_size _dx_rti_type_name_hash_key_callback(_dx_rti_type_name** a) {
  return (*a)->hash_value;
}

static bool _dx_rti_type_name_compare_keys_callback(_dx_rti_type_name** a, _dx_rti_type_name** b) {
  if (*a == *b) {
    return true;
  } else if ((*a)->number_of_bytes == (*b)->number_of_bytes) {
    return !dx_memory_compare((*a)->bytes, (*b)->bytes, (*a)->number_of_bytes);
  }
  return false;
}

static void _dx_rti_type_name_reference(_dx_rti_type_name* a) {
  dx_reference_counter_increment(&a->reference_count);
}

static void _dx_rti_type_name_unreference(_dx_rti_type_name* a) {
  if (0 == dx_reference_counter_decrement(&a->reference_count)) {
    dx_memory_deallocate(a);
  }
}

static void _dx_rti_type_name_reference_callback(_dx_rti_type_name** a) {
  _dx_rti_type_name_reference(*a);
}

static void _dx_rti_type_name_unreference_callback(_dx_rti_type_name** a) {
  _dx_rti_type_name_unreference(*a);
}

static void _dx_rti_type_reference(_dx_rti_type* a) {
  dx_reference_counter_increment(&a->reference_count);
}

static void _dx_rti_type_unreference(_dx_rti_type* a) {
  if (0 == dx_reference_counter_decrement(&a->reference_count)) {
    if (a->on_type_destroyed) {
      a->on_type_destroyed();
    }
    if (_DX_RTI_TYPE_NODE_FLAGS_OBJECT == a->flags & _DX_RTI_TYPE_NODE_FLAGS_OBJECT) {
      if (a->object.parent) {
        _dx_rti_type_unreference(a->object.parent);
        a->object.parent = NULL;
      }
    }
    DX_UNREFERENCE(a->name);
    a->name = NULL;
    dx_memory_deallocate(a);
  }
}

static void _dx_rti_type_reference_callback(_dx_rti_type** a) {
  _dx_rti_type_reference(*a);
}

static void _dx_rti_type_unreference_callback(_dx_rti_type** a) {
  _dx_rti_type_unreference(*a);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_pointer_hashmap* g_types;

int dx_rti_initialize() {
  static DX_POINTER_HASHMAP_CONFIGURATION const configuration = {
   .compare_keys_callback = (bool(*)(dx_pointer_hashmap_key*,dx_pointer_hashmap_key*)) & _dx_rti_type_name_compare_keys_callback,
   .hash_key_callback = (dx_size(*)(dx_pointer_hashmap_key*)) &_dx_rti_type_name_hash_key_callback,
   .key_added_callback = (void(*)(dx_pointer_hashmap_key*)) &_dx_rti_type_name_reference_callback,
   .key_removed_callback = (void(*)(dx_pointer_hashmap_key*)) &_dx_rti_type_name_unreference_callback,
   .value_added_callback = (void(*)(dx_pointer_hashmap_value*)) &_dx_rti_type_reference_callback,
   .value_removed_callback = (void(*)(dx_pointer_hashmap_value*)) &_dx_rti_type_unreference_callback,
  };
  g_types = dx_memory_allocate(sizeof(dx_pointer_hashmap));
  if (!g_types) {
    return 1;
  }
  if (dx_pointer_hashmap_initialize(g_types, &configuration)) {
    dx_memory_deallocate(g_types);
    g_types = NULL;
    return 1;
  }
  return 0;
}

void dx_rti_unintialize() {
  dx_pointer_hashmap_uninitialize(g_types);
  dx_memory_deallocate(g_types);
  g_types = NULL;
}

dx_bool dx_rti_type_is_fundamental(dx_rti_type* type) {
  return _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL == (_DX_RTI_TYPE(type)->flags & _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL);
}

dx_bool dx_rti_type_is_enumeration(dx_rti_type* type) {
  return _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION == (_DX_RTI_TYPE(type)->flags & _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION);
}

dx_bool dx_rti_type_is_object(dx_rti_type* type) {
  return _DX_RTI_TYPE_NODE_FLAGS_OBJECT == (_DX_RTI_TYPE(type)->flags & _DX_RTI_TYPE_NODE_FLAGS_OBJECT);
}

dx_rti_type* dx_rti_create_fundamental(char const* p, dx_size n, void (*on_type_destroyed)(), dx_size value_size) {
  _dx_rti_type_name* name = _dx_rti_type_name_create(p, n);
  if (!name) {
    return NULL;
  }
  _dx_rti_type* type = dx_pointer_hashmap_get(g_types, name);
  if (type) {
    dx_log("a type of the name `", sizeof("a type of the name `") - 1);
    dx_log(p, n);
    dx_log("` already exists", sizeof("` already exists") - 1);
    dx_set_error(DX_EXISTS);
    DX_UNREFERENCE(name);
    name = NULL;
    return NULL;
  }
  if (dx_get_error() != DX_NOT_FOUND) {
    return NULL;
  }
  dx_set_error(DX_NO_ERROR);
  type = dx_memory_allocate(sizeof(_dx_rti_type));
  if (!type) {
    return NULL;
  }
  type->on_type_destroyed = on_type_destroyed;
  type->flags = _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL;
  type->name = name;
  //_dx_rti_type_name_reference(type->name);
  type->reference_count = 1;
  type->fundamental.value_size = value_size;
  if (dx_pointer_hashmap_set(g_types, name, type)) {
    _dx_rti_type_name_unreference(type->name);
    type->name = NULL;
    dx_memory_deallocate(type);
    type = NULL;
    return NULL;
  }
  return NULL;
}

dx_rti_type* dx_rti_create_enumeration(char const* p, dx_size n, void (*on_type_destroyed)()) {
  _dx_rti_type_name* name = _dx_rti_type_name_create(p, n);
  if (!name) {
    return NULL;
  }
  _dx_rti_type* type = dx_pointer_hashmap_get(g_types, name);
  if (type) {
    dx_log("a type of the name `", sizeof("a type of the name `") - 1);
    dx_log(p, n);
    dx_log("` already exists", sizeof("` already exists") - 1);
    dx_set_error(DX_EXISTS);
    DX_UNREFERENCE(name);
    name = NULL;
    return NULL;
  }
  if (dx_get_error() != DX_NOT_FOUND) {
    return NULL;
  }
  dx_set_error(DX_NO_ERROR);
  type = dx_memory_allocate(sizeof(_dx_rti_type));
  if (!type) {
    return NULL;
  }
  type->on_type_destroyed = on_type_destroyed;
  type->flags = _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION;
  type->name = name;
  //_dx_rti_type_name_reference(type->name);
  type->reference_count = 1;
  if (dx_pointer_hashmap_set(g_types, name, type)) {
    _dx_rti_type_name_unreference(type->name);
    type->name = NULL;
    dx_memory_deallocate(type);
    type = NULL;
    return NULL;
  }
  return NULL;
}

dx_rti_type* dx_rti_create_object(char const* p, dx_size n, void (*on_type_destroyed)(), dx_size value_size, dx_rti_type* parent, void (*destruct)(void*)) {
  _dx_rti_type_name* name = _dx_rti_type_name_create(p, n);
  if (!name) {
    return NULL;
  }
  _dx_rti_type* type = dx_pointer_hashmap_get(g_types, name);
  if (type) {
    dx_log("a type of the name `", sizeof("a type of the name `") - 1);
    dx_log(p, n);
    dx_log("` already exists", sizeof("` already exists") - 1);
    dx_set_error(DX_EXISTS);
    DX_UNREFERENCE(name);
    name = NULL;
    return NULL;
  }
  if (dx_get_error() != DX_NOT_FOUND) {
    return NULL;
  }
  dx_set_error(DX_NO_ERROR);
  type = dx_memory_allocate(sizeof(_dx_rti_type));
  if (!type) {
    return NULL;
  }
  type->on_type_destroyed = on_type_destroyed;
  type->flags = _DX_RTI_TYPE_NODE_FLAGS_OBJECT;
  type->name = name;
  //_dx_rti_type_name_reference(type->name);
  type->reference_count = 1;
  type->object.value_size = value_size;
  type->object.destruct = destruct;
  type->object.parent = _DX_RTI_TYPE(parent);
  if (type->object.parent) {
    _dx_rti_type_reference(type->object.parent);
  }
  if (dx_pointer_hashmap_set(g_types, name, type)) {
    if (type->object.parent) {
      _dx_rti_type_unreference(type->object.parent);
      type->object.parent = NULL;
    }
    _dx_rti_type_name_unreference(type->name);
    type->name = NULL;
    dx_memory_deallocate(type);
    type = NULL;
    return NULL;
  }
  DX_UNREFERENCE(type);
  return (dx_rti_type*)type;
}

static inline bool _dx_rti_type_is_leq(_dx_rti_type* x, _dx_rti_type* y) {
  if (!x || !y) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  // If the x is an enumeration type or a fundamental type,
  // then it is lower than or equal to y only if x and y are the same type.
  if ((_DX_RTI_TYPE_NODE_FLAGS_ENUMERATION == (x->flags & _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION)) ||
      (_DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL == (x->flags & _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL))) {
    return x == y;
  }
  // Otherwise x is an object type.
  // It can be lower than or equal to y only if y is also an object type.
  if (0 == (x->flags & _DX_RTI_TYPE_NODE_FLAGS_OBJECT)) {
    return false;
  }
  _dx_rti_type* z = x;
  do {
    if (z == y) {
      return true;
    }
    z = _DX_RTI_TYPE(z->object.parent);
  } while (z != NULL);
  return false;
}

bool dx_rti_type_is_leq(dx_rti_type* x, dx_rti_type* y) {
  return _dx_rti_type_is_leq(_DX_RTI_TYPE(x), _DX_RTI_TYPE(y));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_rti_type* g_dx_object_type = NULL;

static void dx_object_on_type_destroyed() {
  g_dx_object_type = NULL;
}

dx_rti_type* dx_object_get_type() {
  if (!g_dx_object_type) {
    g_dx_object_type = dx_rti_create_object("dx.object", sizeof("dx.object") - 1, &dx_object_on_type_destroyed, sizeof(dx_object), NULL, NULL); \
  }
  return g_dx_object_type;
}

void DX_DEBUG_CHECK_MAGIC_BYTES(void* p) {
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  DX_DEBUG_ASSERT(NULL != p);
  dx_object* q = DX_OBJECT(p);
  DX_DEBUG_ASSERT(q->magic_bytes[0] == 66);
  DX_DEBUG_ASSERT(q->magic_bytes[1] == 12);
  DX_DEBUG_ASSERT(q->magic_bytes[2] == 19);
  DX_DEBUG_ASSERT(q->magic_bytes[3] == 82);
#endif
}

dx_object* dx_object_alloc(dx_size size) {
  if (size < sizeof(dx_object)) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  dx_rti_type* type = dx_object_get_type();
  if (!type) {
    return NULL;
  }
  dx_object* object = dx_memory_allocate(size);
  if (!object) {
    return NULL;
  }
  object->reference_count = 1;
  object->type = type;
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  object->magic_bytes[0] = 66;
  object->magic_bytes[1] = 12;
  object->magic_bytes[2] = 19;
  object->magic_bytes[3] = 82;
#endif
  return object;
}

void dx_object_reference(dx_object *object) {
  DX_DEBUG_CHECK_MAGIC_BYTES(object);
  dx_reference_counter_increment(&object->reference_count);
}

void dx_object_unreference(dx_object* object) {
  DX_DEBUG_CHECK_MAGIC_BYTES(object);
  if (!dx_reference_counter_decrement(&object->reference_count)) {
    while (object->type) {
      _dx_rti_type* type = (_dx_rti_type*)object->type;
      if (type->object.destruct) {
        type->object.destruct(object);
      }
      object->type = (dx_rti_type*)type->object.parent;
    }
    dx_memory_deallocate(object);
    object = NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
