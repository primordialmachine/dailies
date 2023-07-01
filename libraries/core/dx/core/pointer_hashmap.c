#include "dx/core/pointer_hashmap.h"

#include "dx/core/memory.h"
#include "dx/core/safe_mul_nx.h"
#include "dx/core/next_power_of_two.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_size dx_get_best_array_size(dx_size current, dx_size additional, size_t least, size_t greatest, bool saturate);

static dx_size dx_get_best_array_size(dx_size current, dx_size additional, size_t least, size_t greatest, bool saturate) {
  dx_error old_error = dx_get_error();
  if (least > greatest) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (DX_SIZE_GREATEST - current < additional) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  size_t new = current;
  if (new < least) {
    new = least;
  }
  new = current + additional;
  size_t new1 = dx_next_power_of_two_sz(new);
  if (dx_get_error()) {
    dx_set_error(old_error);
  }
  new = new1;
  if (new > greatest) {
    if (!saturate) {
      dx_set_error(DX_NOT_FOUND);
      return 1;
    }
    new = greatest;
    if (new < current + additional) {
      dx_set_error(DX_NOT_FOUND);
      return 1;   
    }
  }
  return new;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct _dx_impl_node _dx_impl_node;

typedef struct _dx_impl _dx_impl;

/// @brief Set the capacity of this implemenation.
/// @param self A pointer to this implementation.
/// @param new_capacity The new capacity. Must not be @a 0.
/// @return The zero value on success. A non-zero value on failure.
/// @failure The function has set the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer.
/// - #DX_INVALID_ARGUMENT @a new_capacity is smaller than @a 1 or greater than the greatest capacity.
/// - #DX_ALLOCATION_FAILED an allocation failed.
static int _dx_impl_set_capacity(_dx_impl* self, dx_size new_capacity);

static int _dx_impl_maybe_resize(_dx_impl* self);

/// @brief Initialize this implementation.
/// @param self A pointer to an uninitialized _dx_impl object.
/// @param configuration A pointer to a DX_POINTER_HASHMAP_CONFIGURATION object.
/// @return The zero value on success. A non-zero value on failure.
static inline int _dx_impl_initialize(_dx_impl* self, DX_POINTER_HASHMAP_CONFIGURATION const* configuration);

/// @brief Uninitialize this implementation.
/// @param self A pointer to an initialized _dx_impl object.
static inline void _dx_impl_uninitialize(_dx_impl* self);

/// @brief Clear this implementation.
/// @param self A pointer to this implementation.
/// @return The zero value on success. A non-zero value on failure.
static inline int _dx_impl_clear(_dx_impl* self);

/// @brief Add or update an entry in this hashmap.
/// @param self A pointer to this hashmap.
/// @param key The key.
/// @param value The value.
/// @return The zero value on success. A non-zero value on failure.
static inline int _dx_impl_set(_dx_impl* self, dx_pointer_hashmap_key key, dx_pointer_hashmap_value value, bool replace);

/// @brief Get the value of an entry in this hashmap.
/// @param self A pointer to this hashmap.
/// @param key The key.
/// @return The value on success.
/// The null pointer on failure.
/// @failure This function has set the the error variable. In particular the following error codes are set
/// - #DX_NOT_FOUND no entry for the specified key was found
/// - #DX_INVALID_ARGUMENT @a self was a null pointer
static inline dx_pointer_hashmap_value _dx_impl_get(_dx_impl* self, dx_pointer_hashmap_key key);

/// @brief Remove an entry from this hashmap.
/// @param self A pointer to this hashmap.
/// @param key The key.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular the following error codes are set
/// - #DX_NOT_FOUND no entry for the specified key was found
/// - #DX_INVALID_ARGUMENT @a self was a null pointer.
static inline int _dx_impl_remove(_dx_impl* self, dx_pointer_hashmap_key key);

/// @brief Get the size, in elements.
/// @param self A pointer to this hashmap.
/// @return The size on success. The size may be zero. The zero value is also returned on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self was a null pointer
static inline dx_size _dx_impl_get_size(_dx_impl const* self);

/// @brief Get the capacity, in elements.
/// @param self A pointer to this hashmap.
/// @return The capacity on success. The capacity may be zero. The zero value is also returned on failure.
/// @default-failureIn particular the following error codes are set
/// - #DX_INVALID_ARGUMENT @a self was a null pointer
static inline dx_size _dx_impl_get_capacity(_dx_impl const* self);

/// @brief Get the free capacity, in elements.
/// @param self A pointer to this hashmap.
/// @return The free capacity on success. The free capacity may be zero. The zero value is also returned on failure.
/// @default-failure In particular the following error codes are set
/// - #DX_INVALID_ARGUMENT @a self was a null pointer.
static inline dx_size _dx_impl_get_free_capacity(_dx_impl const* self);

struct _dx_impl_node {
  _dx_impl_node* next;
  /// @brief The hash value.
  dx_size hash_value;
  /// @brief The key.
  dx_pointer_hashmap_key key;
  /// @brief The value.
  dx_pointer_hashmap_value value;
};

typedef _dx_impl_node* _dx_impl_bucket;


struct _dx_impl {
  
  _dx_impl_bucket* buckets;
  /// @brief Size of this hashmap.
  dx_size size;
  /// @brief The capacity of this hashmap.
  dx_size capacity;
  
  /// @brief A pointer to the @a _dx_key_added_callback_impl1 function or a null pointer.
  dx_key_added_callback* key_added_callback;
  /// @brief A pointer to the @a _dx_key_removed_callback_impl1 function or  a null pointer.
  dx_key_removed_callback* key_removed_callback;
  
  /// @brief A pointer to the @a _dx_value_added_callback_impl1 function or a null pointer.
  dx_value_added_callback* value_added_callback;
  /// @brief A pointer to the @a _dx_value_removed_callback_impl1 function or  a null pointer.
  dx_value_removed_callback* value_removed_callback;
  
  /// @brief A pointer to the @a dx_hash_key_callback function or a null pointer.
  dx_hash_key_callback* hash_key_callback;
  
  /// @brief A pointer to the @a dx_compare_keys_callback function or a null pointer.
  dx_compare_keys_callback* compare_keys_callback;

};

#define _DX_IMPL_LEAST_CAPACITY 1

#define _DX_IMPL_GREATEST_CAPACITY (DX_SIZE_GREATEST / sizeof(_dx_impl_bucket))

static _dx_impl_node** _dx_impl_allocate_bucket_array(size_t n) {
  size_t overflow;
  dx_size n_bytes = dx_mul_sz(n, sizeof(_dx_impl_bucket), &overflow);
  if (overflow) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return NULL;
  }
  _dx_impl_bucket* buckets = dx_memory_allocate(n_bytes);
  if (!buckets) {
    return NULL;
  }
  for (dx_size i = 0; i < n; ++i) {
    buckets[i] = NULL;
  }
  return buckets;
}

static int _dx_impl_set_capacity(_dx_impl* self, dx_size new_capacity) {
  if (!self || !new_capacity) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_size old_capacity = self->capacity;
  _dx_impl_node** old_buckets = self->buckets;
  _dx_impl_node** new_buckets = _dx_impl_allocate_bucket_array(new_capacity);
  if (!new_buckets) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  for (dx_size i = 0; i < old_capacity; ++i) {
    _dx_impl_node** old_bucket = &self->buckets[i];
    while (*old_bucket) {
      _dx_impl_node* node = *old_bucket;
      *old_bucket = node->next;
      dx_size new_hash_index = node->hash_value % self->capacity;
      node->next = new_buckets[new_hash_index];
      new_buckets[new_hash_index] = node;
    }
  }
  dx_memory_deallocate(old_buckets);
  self->capacity = new_capacity;
  self->buckets = new_buckets;
  return 0;
}

static int _dx_impl_maybe_resize(_dx_impl* self) {
  if (self->size > self->capacity) {
    dx_size new_capacity = dx_get_best_array_size(self->capacity, 1, _DX_IMPL_LEAST_CAPACITY, _DX_IMPL_GREATEST_CAPACITY, true);
    if (dx_get_error()) {
      return 1;
    }
    return _dx_impl_set_capacity(self, new_capacity);
  }
  return 0;
}

static inline int _dx_impl_initialize(_dx_impl* self, DX_POINTER_HASHMAP_CONFIGURATION const* configuration) {
  if (!self || !configuration) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  static size_t const INITIAL_CAPACITY = 8;
  self->buckets = _dx_impl_allocate_bucket_array(INITIAL_CAPACITY);
  if (!self->buckets) {
    return 1;
  }
  self->size = 0;
  self->capacity = INITIAL_CAPACITY;
  self->key_added_callback = configuration->key_added_callback;
  self->key_removed_callback = configuration->key_removed_callback;
  self->hash_key_callback = configuration->hash_key_callback;
  self->compare_keys_callback = configuration->compare_keys_callback;
  
  self->value_added_callback = configuration->value_added_callback;
  self->value_removed_callback = configuration->value_removed_callback;
  return 0;
}

static inline void _dx_impl_uninitialize(_dx_impl* self) {
  DX_DEBUG_ASSERT(NULL != self);
  _dx_impl_clear(self);
  DX_DEBUG_ASSERT(NULL != self->buckets);
  dx_memory_deallocate(self->buckets);
  self->buckets = NULL;
}

static inline int _dx_impl_clear(_dx_impl* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  for (dx_size i = 0, n = self->capacity; i < n; ++i) {
    _dx_impl_node** bucket = &self->buckets[i];
    while (*bucket) {
      _dx_impl_node* node = *bucket;
      *bucket = node->next;
      if (self->value_removed_callback) {
        self->value_removed_callback(&node->value);
      }
      if (self->key_removed_callback) {
        self->key_removed_callback(&node->key);
      }
      dx_memory_deallocate(node);
    }
  }
  self->size = 0;
  return 0;
}

static inline int _dx_impl_set(_dx_impl* self, dx_pointer_hashmap_key key, dx_pointer_hashmap_value value, bool replace) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_size hash_value = self->hash_key_callback(&key);
  dx_size hash_index = hash_value % self->capacity;
  _dx_impl_node* node;
  for (node = self->buckets[hash_index]; NULL != node; node = node->next) {
    if (node->hash_value == hash_value) {
      if (self->compare_keys_callback(&node->key, &key)) {
        break;
      }
    }
  }
  if (node) {
    if (!replace) {
      dx_set_error(DX_EXISTS);
      return 1;
    }
    //
    if (self->key_added_callback) {
      self->key_added_callback(&key);
    }
    if (self->key_removed_callback) {
      self->key_removed_callback(&node->key);
    }
    node->key = key;
    //
    if (self->value_added_callback) {
      self->value_added_callback(&value);
    }
    if (self->value_removed_callback) {
      self->value_removed_callback(&node->value);
    }
    node->value = value;
  } else {
    node = dx_memory_allocate(sizeof(_dx_impl_node));
    if (!node) {
      return 1;
    }
    if (self->key_added_callback) {
      self->key_added_callback(&key);
    }
    node->key = key;
    if (self->value_added_callback) {
      self->value_added_callback(&value);
    }
    node->value = value;
    node->hash_value = hash_value;
    node->next = self->buckets[hash_index];
    self->buckets[hash_index] = node;
    self->size++;
    dx_error old_error = dx_get_error();
    if (_dx_impl_maybe_resize(self)) {
      dx_set_error(old_error);
    }
  }
  return 0;
}

static inline dx_pointer_hashmap_value _dx_impl_get(_dx_impl* self, dx_pointer_hashmap_key key) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  dx_size hash_value = self->hash_key_callback(&key);
  dx_size hash_index = hash_value % self->capacity;
  _dx_impl_node* node;
  for (node = self->buckets[hash_index]; NULL != node; node = node->next) {
    if (node->hash_value == hash_value) {
      if (self->compare_keys_callback(&node->key, &key)) {
        return node->value;
      }
    }
  }
  dx_set_error(DX_NOT_FOUND);
  return NULL;
}

static inline int _dx_impl_remove(_dx_impl* self, dx_pointer_hashmap_key key) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  dx_size hash_value = self->hash_key_callback(&key);
  dx_size hash_index = hash_value % self->capacity;
  _dx_impl_node** previous;
  _dx_impl_node* current;
  for (previous = &self->buckets[hash_index], current = self->buckets[hash_index]; NULL != current; previous = &current->next, current = current->next) {
    if (current->hash_value == hash_value) {
      if (self->compare_keys_callback(&current->key, &key)) {
        *previous = current->next;
        if (self->value_removed_callback) {
          self->value_removed_callback(&current->value);
        }
        if (self->key_removed_callback) {
          self->key_removed_callback(&current->key);
        }
        dx_memory_deallocate(current);
        return 0;
      }
    }
  }
  dx_set_error(DX_NOT_FOUND);
  return 1;
}

static inline dx_size _dx_impl_get_size(_dx_impl const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return self->size;
}

static inline dx_size _dx_impl_get_capacity(_dx_impl const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return self->capacity;
}

static inline dx_size _dx_impl_get_free_capacity(_dx_impl const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return self->capacity - self->size;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline _dx_impl* _DX_IMPL(void* p) {
  return (_dx_impl*)p;
}

int dx_pointer_hashmap_initialize(dx_pointer_hashmap* self, DX_POINTER_HASHMAP_CONFIGURATION const* configuration) {
  if (!self || !configuration) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->pimpl = dx_memory_allocate(sizeof(_dx_impl));
  if (!self->pimpl) {
    return 1;
  }
  if (_dx_impl_initialize(_DX_IMPL(self->pimpl), configuration)) {
    dx_memory_deallocate(self->pimpl);
    self->pimpl = NULL;
    return 1;
  }
  return 0;
}

void dx_pointer_hashmap_uninitialize(dx_pointer_hashmap* self) {
  DX_DEBUG_ASSERT(NULL != self);
  DX_DEBUG_ASSERT(NULL != self->pimpl);
  _dx_impl_uninitialize(_DX_IMPL(self->pimpl));
  dx_memory_deallocate(self->pimpl);
  self->pimpl = NULL;
}

int dx_pointer_hashmap_clear(dx_pointer_hashmap* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return _dx_impl_clear(_DX_IMPL(self->pimpl));
}

int dx_pointer_hashmap_set(dx_pointer_hashmap* self, dx_pointer_hashmap_key key, dx_pointer_hashmap_value value) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return _dx_impl_set(_DX_IMPL(self->pimpl), key, value, true);
}

void* dx_pointer_hashmap_get(dx_pointer_hashmap const* self, dx_pointer_hashmap_key key) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  return _dx_impl_get(_DX_IMPL(self->pimpl), key);
}

int dx_pointer_hashmap_remove(dx_pointer_hashmap* self, dx_pointer_hashmap_key key) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return _dx_impl_remove(_DX_IMPL(self->pimpl), key);
}

dx_size dx_pointer_hashmap_get_size(dx_pointer_hashmap const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return _dx_impl_get_size(_DX_IMPL(self->pimpl));
}

dx_size dx_pointer_hashmap_get_capacity(dx_pointer_hashmap const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return _dx_impl_get_capacity(_DX_IMPL(self->pimpl));
}

dx_size dx_pointer_hashmap_get_free_capacity(dx_pointer_hashmap const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return _dx_impl_get_free_capacity(_DX_IMPL(self->pimpl));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct _dx_impl_iterator _dx_impl_iterator;
static inline _dx_impl_iterator* _DX_IMPL_ITERATOR(void* p) {
 return (_dx_impl_iterator*)p; 
}
struct _dx_impl_iterator {
  _dx_impl* target;
  dx_size bucket;
  _dx_impl_node** previous;
  _dx_impl_node* current;
};

static inline void _dx_impl_increment(_dx_impl_iterator* self);

static inline void _dx_impl_increment(_dx_impl_iterator* self) {
  if (self->current) {
    self->previous = &self->current->next;
    self->current = self->current->next;
  } else {
    if (self->bucket < self->target->capacity) {
      do {
        self->previous = &self->target->buckets[self->bucket];
        self->current = self->target->buckets[self->bucket];
        if (self->current != NULL) {
          break;
        }
        self->bucket++;
      } while (self->bucket < self->target->capacity);
    }
  }
}

int dx_pointer_hashmap_iterator_initialize(dx_pointer_hashmap_iterator* self, dx_pointer_hashmap* target) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  _dx_impl_iterator* pimpl = dx_memory_allocate(sizeof(_dx_impl_iterator));
  if (!pimpl) {
    return 1;
  }
  pimpl->target = _DX_IMPL(target->pimpl);
  pimpl->bucket = 0;
  pimpl->previous = NULL;
  pimpl->current = NULL;
  _dx_impl_increment(pimpl);
  self->pimpl = pimpl;
  return 0;
}

void dx_pointer_hashmap_iterator_uninitialize(dx_pointer_hashmap_iterator* self) {
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(self->pimpl);
  dx_memory_deallocate(pimpl);
}

int dx_pointer_hashmap_iterator_next(dx_pointer_hashmap_iterator* self) {
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(self->pimpl);
  _dx_impl_increment(pimpl);
  return 0;
}

bool dx_pointer_hashmap_iterator_has_entry(dx_pointer_hashmap_iterator* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(self->pimpl);
  return NULL != pimpl->current;
}

void* dx_pointer_hashmap_iterator_get_value(dx_pointer_hashmap_iterator* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(self->pimpl);
  if (!pimpl->current) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  return pimpl->current->value;
}

void* dx_pointer_hashmap_iterator_get_key(dx_pointer_hashmap_iterator* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(self->pimpl);
  if (!pimpl->current) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  return pimpl->current->key;
}

int dx_pointer_hashmap_iterator_remove(dx_pointer_hashmap_iterator* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(self->pimpl);
  if (!pimpl->current) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  _dx_impl_node** previous = pimpl->previous;
  _dx_impl_node* current = pimpl->current;
  // Predecessor points to successor.
  *previous = current->next;
  // Decrement size by 1.
  pimpl->target->size--;
  // Update iterator.
  pimpl->previous = previous;
  pimpl->current = *previous;
  if (!pimpl->current) {
    // Only if the removed node was the last in the bucket and hence pimpl->current is now a null pointer.
    _dx_impl_increment(pimpl);
  }
  return 0;
}
