// Copyright (c) 2018-2023 Michael Heilmann
#if !defined(DX_CORE_POINTER_HASHMAP_H_INCLUDED)
#define DX_CORE_POINTER_HASHMAP_H_INCLUDED

#include "dx/core/core.h"

typedef void* dx_pointer_hashmap_key;
typedef void* dx_pointer_hashmap_value;

/// @brief Type of a callback function invoked if a key was added.
/// @param key A pointer to the key.
typedef void (dx_key_added_callback)(dx_pointer_hashmap_key* key);

/// @brief Type of a callback function invoked if a key was removed.
/// @param key A pointer to the key.
typedef void (dx_key_removed_callback)(dx_pointer_hashmap_key*);

/// @brief Type of a callback function invoked if a value was added.
/// @param key A pointer to the value.
typedef void (dx_value_added_callback)(dx_pointer_hashmap_value*);

/// @brief Type of a callback function invoked if a value was removed.
/// @param key A pointer to the value.
typedef void (dx_value_removed_callback)(dx_pointer_hashmap_value*);

/// @brief Type of a callback function invoked if the hash value of a key must be computed.
/// @param key A pointer to the key
typedef dx_size (dx_hash_key_callback)(dx_pointer_hashmap_key*);
typedef dx_bool (dx_compare_keys_callback)(dx_pointer_hashmap_key*, dx_pointer_hashmap_key*);

typedef struct dx_pointer_hashmap_node dx_pointer_hashmap_node;
struct dx_pointer_hashmap_node {
  dx_pointer_hashmap_node* next;
  dx_size hash_value;
  dx_pointer_hashmap_key key;
  dx_pointer_hashmap_value value;
};

/// @brief A dynamic hashmap of pointers (also null pointers).
/// Supports callbacks for notifications on additions and removals of pointers.
typedef struct dx_pointer_hashmap dx_pointer_hashmap;

struct dx_pointer_hashmap {
  void* pimpl;
};

typedef struct DX_POINTER_HASHMAP_CONFIGURATION {

  /// @brief A pointer to the @a _dx_key_added_callback function or a null pointer.
  dx_key_added_callback* key_added_callback;
  /// @brief A pointer to the @a _dx_key_removed_callback function or  a null pointer.
  dx_key_removed_callback* key_removed_callback;
  
  /// @brief A pointer to the @a _dx_value_added_callback function or a null pointer.
  dx_value_added_callback* value_added_callback;
  /// @brief A pointer to the @a _dx_value_removed_callback function or  a null pointer.
  dx_value_removed_callback* value_removed_callback;
  
  /// @brief A pointer to the @a dx_hash_key_callback function or a null pointer.
  dx_hash_key_callback* hash_key_callback;
  
  /// @brief A pointer to the @a dx_compare_keys_callback function or a null pointer.
  dx_compare_keys_callback* compare_keys_callback;
  
} DX_POINTER_HASHMAP_CONFIGURATION;

/// @brief Initialize this hashmap.
/// @param self A pointer to this hashmap.
/// @param configuration A pointer to the configuration.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable.
int dx_pointer_hashmap_initialize(dx_pointer_hashmap* self, DX_POINTER_HASHMAP_CONFIGURATION const* configuration);

/// @brief Uninitialize this hashmap.
/// @param self A pointer to this hashmap.
void dx_pointer_hashmap_uninitialize(dx_pointer_hashmap* self);

/// @brief Clear this hashmap.
/// @param self A pointer to this hashmap.
/// @return The zero value on success. A non-zero value on failure.
int dx_pointer_hashmap_clear(dx_pointer_hashmap* self);

/// @brief Add or update an entry in this hashmap.
/// @param self A pointer to this hashmap.
/// @param key The key.
/// @param value The value.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular the following error codes are set
/// - #DX_INVALID_ARGUMENT @a self was a null pointer.
/// - #DX_ALLOCATION_FAILED An allocation failed.
/// - #DX_EXISTS An entry for the specified key already exists.
int dx_pointer_hashmap_set(dx_pointer_hashmap* self, dx_pointer_hashmap_key key, dx_pointer_hashmap_value value);

/// @brief Get the value of an entry in this hashmap.
/// @param self A pointer to this hashmap.
/// @param key The key.
/// @return The value on success.
/// The null pointer on failure.
/// @failure This function has set the the error variable. In particular the following error codes are set
/// - #DX_NOT_FOUND no entry for the specified key was found
/// - #DX_INVALID_ARGUMENT @a self was a null pointer
dx_pointer_hashmap_value dx_pointer_hashmap_get(dx_pointer_hashmap const* self, dx_pointer_hashmap_key key);

/// @brief Remove an entry from this hashmap.
/// @param self A pointer to this hashmap.
/// @param key The key.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular the following error codes are set
/// - #DX_NOT_FOUND no entry for the specified key was found
/// - #DX_INVALID_ARGUMENT @a self was a null pointer
int dx_pointer_hashmap_remove(dx_pointer_hashmap* self, dx_pointer_hashmap_key key);

/// @brief Get the size, in elements.
/// @param self A pointer to this hashmap.
/// @return The size on success. The size may be zero. The zero value is also returned on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size dx_pointer_hashmap_get_size(dx_pointer_hashmap const* self);

/// @brief Get the capacity, in elements.
/// @param self A pointer to this hashmap.
/// @return The capacity on success. The capacity may be zero. The zero value is also returned on failure.
/// @default-failureIn particular the following error codes are set
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size dx_pointer_hashmap_get_capacity(dx_pointer_hashmap const* self);

/// @brief Get the free capacity, in elements.
/// @param self A pointer to this hashmap.
/// @return The free capacity on success. The free capacity may be zero. The zero value is also returned on failure.
/// @default-failure In particular the following error codes are set
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size dx_pointer_hashmap_get_free_capacity(dx_pointer_hashmap const* self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief An iterator for a pointer hash map.
/// @warning An iterator is invalidated if the hashmap is modified through any other function other than dx_pointer_hashmap_iterator_remove for that particular iterator. 
/// @detail
/// Can be used as follows.
/// @code
/// dx_pointer_hashmap_iterator iterator;
/// dx_pointer_hashmap target;
/// ...
/// if (dx_pointer_hashmap_iterator_initialize(&target, &target)) { ... }
/// dx_set_error(DX_NO_ERROR);
/// while (dx_pointer_hashmap_iterator_has_entry(&target)) {
///   void* k = dx_pointer_hashmap_iterator_get_key(&target),
///       * v = dx_pointer_hashmap_iterator_get_value(&target);
///   if (dx_get_error()) { ... }
///   ...
///   if (dx_pointer_hashmap_iterator_next(&target)) { ...  }
/// }
/// dx_pointer_hashmap_iterator_uninitialize(&target);
/// @endcode
typedef struct dx_pointer_hashmap_iterator dx_pointer_hashmap_iterator;

struct dx_pointer_hashmap_iterator {
  void* pimpl;
};

/// @brief Initialize this pointer hashmap iterator.
/// @param self A pointer to the dx_pointer_hashmap_iterator object.
/// @param target A pointer to the pointer hashmap to iterate over.
/// @return The zero value on success. A non-zero value on failure.
int dx_pointer_hashmap_iterator_initialize(dx_pointer_hashmap_iterator* self, dx_pointer_hashmap* target);

/// @brief Uninitialize this pointer hashmap iterator.
/// @param self A pointer to the dx_pointer_hashmap_iterator object.
void dx_pointer_hashmap_iterator_uninitialize(dx_pointer_hashmap_iterator* self);

/// @brief Inc
/// @return The zero value on success. A non-zero value on failure.
int dx_pointer_hashmap_iterator_next(dx_pointer_hashmap_iterator* self);

/// @return @a true if this pointer hashmap iterator has an entry. @a false if it does not have an entry. @a false is also returned on failure.
/// @default-failure
bool dx_pointer_hashmap_iterator_has_entry(dx_pointer_hashmap_iterator* self);

/// @return The key on success. A null pointer is also returned on failure.
/// @default-failure
void* dx_pointer_hashmap_iterator_get_value(dx_pointer_hashmap_iterator* self);

/// @return The key on success. A null pointer is also returned on failure.
/// @default-failure
void* dx_pointer_hashmap_iterator_get_key(dx_pointer_hashmap_iterator* self);

int dx_pointer_hashmap_iterator_remove(dx_pointer_hashmap_iterator* self);

#endif // DX_CORE_POINTER_HASHMAP_H_INCLUDED