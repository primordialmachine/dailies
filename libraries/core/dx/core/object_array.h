#if !defined(DX_CORE_OBJECT_ARRAY_H_INCLUDED)
#define DX_CORE_OBJECT_ARRAY_H_INCLUDED

#include "dx/core/pointer_array.h"
#include "dx/core/object.h"

/// @brief A dynamic array of (pointers to) objects. Also supports null pointers.
/// Support for reference counting objects.
typedef struct dx_object_array dx_object_array;
static inline dx_object_array* DX_OBJECT_ARRAY(void* p) {
  return (dx_object_array*)p;
}

struct dx_object_array {
  dx_pointer_array backend;
};

/// @brief Initialize this dx_object_array object.
/// @param self A pointer to this dx_object_array object.
/// @param initial_capacity The initialc capacity.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED @a initial_capacity is too big
/// - #DX_ALLOCATION_FAILED an allocation failed
int dx_object_array_initialize(dx_object_array* self, dx_size initial_capacity);

/// @brief Uninitialize this dx_object_array object.
/// @param self A pointer to this dx_object_array object.
void dx_object_array_uninitialize(dx_object_array* self);

/// @brief Increase the capacity.
/// @param self A pointer to this dx_object_array object.
/// @param additional_capacity The amount to increase the capacity by.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED @a additional_capacity is too big
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
int
dx_object_array_increase_capacity
  (
    dx_object_array* self,
    dx_size additional_capacity
  );

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param self A pointer to this dx_object_array object.
/// @param required_free_capacitiy The required free capacity.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED @a required_free_capacity is too big
/// - #DX_ALLOCATION_FAILED an allocation failed
int
dx_object_array_ensure_free_capacity
  (
    dx_object_array* self,
    dx_size required_free_capacity
  );

/// @brief Append an element.
/// @param self A pointer to this dx_object_array object.
/// @param pointer The element.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a pointer is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int
dx_object_array_append
  (
    dx_object_array* self,
    dx_object* pointer
  );

/// @brief Prepend an element.
/// @param self A pointer to this dx_object_array object.
/// @param pointer The element.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a pointer is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int
dx_object_array_prepend
  (
    dx_object_array* self,
    dx_object* pointer
  );

/// @brief Insert an element.
/// @param self A pointer to this dx_object_array object.
/// @param pointer The element.
/// @param index The index.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a pointer is a null pointer
/// - #DX_INVALID_ARGUMENT @a index is greater than the size of this dx_object_array object
/// - #DX_ALLOCATION_FAILED an allocation failed
int
dx_object_array_insert
  (
    dx_object_array* self,
    dx_object* pointer,
    dx_size index
  );

/// @brief Get the pointer at the specified index.
/// @param self A pointer to this pointer array.
/// @param index The index.
/// @return The pointer at the specified index on success. This might be the null pointer.
/// The null pointer is also returned on failure.
/// @default-failure
dx_object*
dx_object_array_get_at
  (
    dx_object_array* self,
    dx_size index
  );

/// @brief Get the size, in elements.
/// @param self A pointer to this dx_object_array object.
/// @return The size on success. The size may be zero. The zero value is also returned on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size
dx_object_array_get_size
  (
    dx_object_array const* self
  );

/// @brief Get the capacity, in elements.
/// @param self A pointer to this dx_object_array object.
/// @return The capacity on success. The capacity may be zero. The zero value is also returned on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size
dx_object_array_get_capacity
  (
    dx_object_array const* self
  );

/// @brief Get the free capacity, in elements.
/// @param self A pointer to this dx_object_array object.
/// @return The free capacity on success. The free capacity may be zero. The zero value is also returned on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size
dx_object_array_get_free_capacity
  (
    dx_object_array const* self
  );

/// @brief Remove all elements.
/// @param self A pointer to this dx_object_array object.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
int
dx_object_array_clear
  (
    dx_object_array* self
  );

#endif // DX_CORE_OBJECT_ARRAY_H_INCLUDED
