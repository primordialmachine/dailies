// Copyright (c) 2018-2023 Michael Heilmann
#if !defined(DX_CORE_POINTER_ARRAY_H_INCLUDED)
#define DX_CORE_POINTER_ARRAY_H_INCLUDED

#include "dx/core/core.h"

/// @brief The elements of a dx_core_pointer_array are pointers.
typedef void* dx_pointer_array_element;

/// @brief Type of a callback invoked if an element was added.
/// @param element A pointer to the element.
typedef void (dx_added_callback)(dx_pointer_array_element* element);

/// @brief Type of a callback invoked if an element was removed.
/// @param element A pointer to the element.
typedef void (dx_removed_callback)(dx_pointer_array_element* element);

/// @brief A dynamic array of pointers (also null pointers).
/// Supports callbacks for notifications on additions and removals of pointers.
typedef struct dx_pointer_array dx_pointer_array;

struct dx_pointer_array {

  /// @brief A pointer to an array of @a capacity @a (void *) elements.
  void **elements;
  /// @brief The capacity, in elements, of the array pointed to by @a array.
  dx_size capacity;
  /// @brief The number of elements in this array.
  dx_size size;

  /// @brief A pointer to the @a dx_added_callback function or a null pointer.
  dx_added_callback *added_callback;
  /// @brief A pointer to the @a dx_removed_callback function or  a null pointer.
  dx_removed_callback *removed_callback;

}; // struct dx_pointer_array

/// @brief Initialize this dx_pointer_array object.
/// @param self A pointer to this dx_pointer_array object.
/// @param initial_capacity The initialc capacity.
/// @param added_callback A pointer to a @a dx_added_callback or a null pointer.
/// @param removed_callback A pointer to a @a dx_removed_callback or a null pointer.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED @a initial_capacity is too big
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @remarks
/// If an added or removed callback is supplied, then it will receive a pointer to the pointer that was added or removed, respectively.
int
dx_pointer_array_initialize
  (
    dx_pointer_array* self,
    dx_size initial_capacity,
    dx_added_callback* reference,
    dx_removed_callback* unreference
  );

/// @brief Uninitialize this dx_pointer_array object.
/// @param self A pointer to this dx_pointer_array object.
void
dx_pointer_array_uninitialize
  (
    dx_pointer_array* self
  );

/// @brief Increase the capacity.
/// @param self A pointer to this dx_pointer_array object.
/// @param additional_capacity The amount to increase the capacity by.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED @a additional_capacity is too big
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
int
dx_pointer_array_increase_capacity
  (
    dx_pointer_array* self,
    dx_size additional_capacity
  );

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param self A pointer to this dx_pointer_array object.
/// @param required_free_capacitiy The required free capacity.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED @a required_free_capacity is too big
/// - #DX_ALLOCATION_FAILED an allocation failed
int
dx_pointer_array_ensure_free_capacity
  (
    dx_pointer_array* self,
    dx_size required_free_capacity
  );

/// @brief Append an element.
/// @param self A pointer to this dx_pointer_array object.
/// @param pointer The element.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a pointer is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int
dx_pointer_array_append
  (
    dx_pointer_array* self,
    dx_pointer_array_element pointer
  );

/// @brief Prepend an element.
/// @param self A pointer to this dx_pointer_array object.
/// @param pointer The element.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a pointer is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int
dx_pointer_array_prepend
  (
    dx_pointer_array* self,
    dx_pointer_array_element pointer
  );

/// @brief Insert an element.
/// @param self A pointer to this dx_pointer_array object.
/// @param pointer The element.
/// @param index The index.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a pointer is a null pointer
/// - #DX_INVALID_ARGUMENT @a index is greater than the size of this dx_pointer_array object
/// - #DX_ALLOCATION_FAILED an allocation failed
int
dx_pointer_array_insert
  (
    dx_pointer_array* self,
    dx_pointer_array_element pointer,
    dx_size index
  );

/// @brief Get the pointer at the specified index.
/// @param self A pointer to this pointer array.
/// @param index The index.
/// @return The pointer at the specified index on success. This might be the null pointer.
/// The null pointer is also returned on failure.
/// @failure This function has set the error variable.
void*
dx_pointer_array_get_at
  (
    dx_pointer_array* self,
    size_t index
  );

/// @brief Get the size, in elements.
/// @param self A pointer to this dx_pointer_array object.
/// @return The size on success. The size may be zero. The zero value is also returned on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size
dx_pointer_array_get_size
  (
    dx_pointer_array const* self
  );

/// @brief Get the capacity, in elements.
/// @param self A pointer to this dx_pointer_array object.
/// @return The capacity on success. The capacity may be zero. The zero value is also returned on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size
dx_pointer_array_get_capacity
  (
    dx_pointer_array const* self
  );

/// @brief Get the free capacity, in elements.
/// @param self A pointer to this dx_pointer_array object.
/// @return The free capacity on success. The free capacity may be zero. The zero value is also returned on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size
dx_pointer_array_get_free_capacity
  (
    dx_pointer_array const* self
  );

/// @brief Remove all elements.
/// @param self A pointer to this dx_pointer_array object.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
int
dx_pointer_array_clear
  (
    dx_pointer_array* self
  );

#endif // DX_CORE_POINTER_ARRAY_H_INCLUDED
