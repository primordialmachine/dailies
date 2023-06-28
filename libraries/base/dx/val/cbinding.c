#include "dx/val/cbinding.h"

// strcmp
#include <string.h>

typedef struct _entry _entry;

typedef struct _map _map;

struct _entry {
  _entry* next;
  size_t hv;
  char* name;
  uint8_t tag;
  union {
    DX_VEC3 vec3;
    DX_VEC4 vec4;
    DX_MAT4 mat4;
    size_t texture_index;
  };
};

struct _map{
  _entry** buckets;
  size_t size;
  size_t capacity;
};

static size_t hash(char const* p, size_t n) {
  size_t hv = n;
  for (size_t i = n; i > 0; --i) {
    hv = hv * 37 + (size_t)(p[i - 1]);
  }
  return hv;
}

static void* _create_impl();

static void _destroy_impl(void* pimpl);

static _entry* get_or_create_impl(_map* map, bool create, char const* name);

static void* _create_impl() {
  _map* map = dx_memory_allocate(sizeof(_map));
  if (!map) {
    return NULL;
  }
  map->size = 0;
  map->capacity = 8;
  map->buckets = dx_memory_allocate(sizeof(_entry) * map->capacity);
  if (!map->buckets) {
    dx_memory_deallocate(map);
    map = NULL;
    return NULL;
  }
  for (size_t i = 0, n = map->capacity; i < n; ++i) {
    map->buckets[i] = NULL;
  }
  return map;
}

static void _destroy_impl(void* pimpl) {
  _map* map = (_map*)pimpl;
  for (size_t i = 0, n = map->capacity; i < n; ++i) {
    while (map->buckets[i]) {
      _entry* entry = map->buckets[i];
      map->buckets[i] = entry->next;
      dx_memory_deallocate(entry->name);
      entry->name = NULL;
      dx_memory_deallocate(entry);
      map->size--;
    }
  }
  dx_memory_deallocate(map->buckets);
  map->buckets = NULL;
  dx_memory_deallocate(map);
}

static _entry* get_or_create_impl(_map* map, bool create, char const* name) {
  size_t n = strlen(name);
  size_t hv = hash(name, n);
  size_t hi = hv % map->capacity;
  for (_entry* entry = map->buckets[hi]; NULL != entry; entry = entry->next) {
    if (entry->hv == hv) {
      if (!strcmp(entry->name, name)) {
        return entry;
      }
    }
  }
  if (!create) {
    return NULL;
  }
  _entry* entry = dx_memory_allocate(sizeof(_entry));
  if (!entry) {
    return NULL;
  }
  entry->hv = hv;
  entry->name = _strdup(name);
  if (!entry->name) {
    dx_set_error(DX_ALLOCATION_FAILED);
    dx_memory_deallocate(entry);
    entry = NULL;
    return NULL;
  }
  entry->tag = DX_CBINDING_TYPE_EMPTY;
  entry->next = map->buckets[hi];
  map->buckets[hi] = entry;
  map->size++;
  return entry;
}

int dx_cbinding_set_vec3(dx_cbinding* self, char const* name, DX_VEC3 const* v) {
  _entry* entry = get_or_create_impl((_map*)self->pimpl, true, name);
  if (!entry) {
    return 1;
  }
  entry->tag = DX_CBINDING_TYPE_VEC3;
  entry->vec3 = *v;
  return 0;
}

int dx_cbinding_set_vec4(dx_cbinding* self, char const* name, DX_VEC4 const* v) {
  _entry* entry = get_or_create_impl((_map*)self->pimpl, true, name);
  if (!entry) {
    return 1;
  }
  entry->tag = DX_CBINDING_TYPE_VEC4;
  entry->vec4 = *v;
  return 0;
}

int dx_cbinding_set_mat4(dx_cbinding* self, char const *name, DX_MAT4 const* a) {
  _entry* entry = get_or_create_impl((_map*)self->pimpl, true, name);
  if (!entry) {
    return 1;
  }
  entry->tag = DX_CBINDING_TYPE_MAT4;
  entry->mat4 = *a;
  return 0;
}

int dx_cbinding_set_texture_index(dx_cbinding* self, char const* name, size_t i) {
  _entry* entry = get_or_create_impl((_map*)self->pimpl, true, name);
  if (!entry) {
    return 1;
  }
  entry->tag = DX_CBINDING_TYPE_TEXTURE_INDEX;
  entry->texture_index = i;
  return 0;
}

DX_DEFINE_OBJECT_TYPE("dx.cbinding",
                      dx_cbinding,
                      dx_object)

int dx_cbinding_construct(dx_cbinding* self) {
  dx_rti_type* _type = dx_cbinding_get_type();
  if (!_type) {
    return 1;
  }
  self->pimpl = _create_impl();
  if (!self->pimpl) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_cbinding_destruct(dx_cbinding* self) {
  _destroy_impl(self->pimpl);
  self->pimpl = NULL;
}

dx_cbinding* dx_cbinding_create() {
  dx_cbinding* self = DX_CBINDING(dx_object_alloc(sizeof(dx_cbinding)));
  if (!self) {
    return NULL;
  }
  if (dx_cbinding_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

dx_cbinding_iter dx_cbinding_get_iter(dx_cbinding* self) {
  _map* map = (_map*)self->pimpl;
  for (size_t i = 0, n = map->capacity; i < n; ++i) {
    if (NULL != map->buckets[i]) {
      dx_cbinding_iter it = { .a = (void*)i, .b = map->buckets[i], .c = map };
      return it;
    }
  }
  dx_cbinding_iter it = { .a = (void*)map->capacity, .b = NULL, .c = map };
  return it;
}

int dx_cbinding_iter_next(dx_cbinding_iter* self) {
  _map* map = (_map*)self->c;
  _entry* entry = (_entry*)self->b;
  if (entry) {
    entry = entry->next;
    if (entry) {
      // Found successor in current bucket. Done.
      self->b = (void*)entry;
      return 0;
    } else {
      // Found NO successor in current bucket. move to next non-empty bucket if any.
      size_t index = (size_t)self->a;
      index++;
      for (; index < map->capacity && !map->buckets[index]; ++index) {

      }
      if (index == map->capacity) {
        // Found no next non-empty bucket. Done.
        self->a = (void*)index;
        self->b = NULL;
        return 0;
      } else {
        // Found next non-empty bucket. Done.
        self->a = (void*)index;
        self->b = map->buckets[index];
        return 0;
      }
    }
  } else {
    // Is already at end. Done.
    return 0;
  }
}

bool dx_cbinding_iter_has_value(dx_cbinding_iter const* self) {
  _entry* entry = (_entry*)self->b;
  return NULL != entry;
}

uint8_t dx_cbinding_iter_get_tag(dx_cbinding_iter const* self) {
  _entry* entry = (_entry*)self->b;
  if (!entry) {
    dx_set_error(DX_INVALID_OPERATION);
    return DX_CBINDING_TYPE_EMPTY;
  }
  return entry->tag;
}

char const* dx_cbinding_iter_get_name(dx_cbinding_iter const* self) {
  _entry* entry = (_entry*)self->b;
  if (!entry) {
    dx_set_error(DX_INVALID_OPERATION);
    return DX_CBINDING_TYPE_EMPTY;
  }
  return entry->name;
}

int dx_cbinding_iter_get_vec3(dx_cbinding_iter const* self, DX_VEC3* v) {
  _entry* entry = (_entry*)self->b;
  if (!entry) {
    dx_set_error(DX_INVALID_OPERATION);
    return DX_CBINDING_TYPE_EMPTY;
  }
  *v = entry->vec3;
  return 0;
}

int dx_cbinding_iter_get_vec4(dx_cbinding_iter const* self, DX_VEC4* v) {
  _entry* entry = (_entry*)self->b;
  if (!entry) {
    dx_set_error(DX_INVALID_OPERATION);
    return DX_CBINDING_TYPE_EMPTY;
  }
  *v = entry->vec4;
  return 0;
}

int dx_cbinding_iter_get_mat4(dx_cbinding_iter const* self, DX_MAT4* a) {
  _entry* entry = (_entry*)self->b;
  if (!entry) {
    dx_set_error(DX_INVALID_OPERATION);
    return DX_CBINDING_TYPE_EMPTY;
  }
  *a = entry->mat4;
  return 0;
}

int dx_cbinding_iter_get_texture_index(dx_cbinding_iter const* self, size_t* i) {
  _entry* entry = (_entry*)self->b;
  if (!entry) {
    dx_set_error(DX_INVALID_OPERATION);
    return DX_CBINDING_TYPE_EMPTY;
  }
  *i = entry->texture_index;
  return 0;
}
