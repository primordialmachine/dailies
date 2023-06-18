#if !defined(DX_CORE_BYTE_BUFFER_H_INCLUDED)
#define DX_CORE_BYTE_BUFFER_H_INCLUDED

#include "dx/core/core.h"

typedef struct dx_byte_array {

  /// @brief A pointer to an array of @a capacity @a (char) elements.
  /// The first @a size @a (char) elements are the contents of the buffer.
  char *elements;

  /// @brief The size, in elements, of this buffer.
  dx_size size;

  /// @brief The capacity, in elements, of this buffer.
  dx_size capacity;

} dx_byte_array;

/// @brief Initialize this dx_byte_array object.
/// @param self A pointer to this dx_byte_array object.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int dx_byte_array_initialize(dx_byte_array* self);

/// @brief Uninitialize this dx_byte_array object.
/// @param self A pointer to this dx_byte_array object.
void dx_byte_array_uninitialize(dx_byte_array* self);

/// @brief "steal" the backing Byte array from this dx_byte_array object.
/// @param bytes A pointer to a <code>char</code> variable.
/// @param number_of_bytes A pointer to <code>dx_size</code> variable.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a bytes is a null pointer
/// - #DX_INVALID_ARGUMENT @a number_of_bytes is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @success
/// <code>*bytes</code> was assigned a pointer to an array of <code>*number_of_bytes</code> Bytes.
/// <code>*number_of_bytes</code> was assigned the number of Bytes.
/// The array pointed to by <code>*bytes</code> must be deallocated using <code>free(void*)</code>.
int dx_byte_array_steal(dx_byte_array* self, char **bytes, dx_size *number_of_bytes);

/// @brief Increase the capacity.
/// @param self A pointer to this dx_byte_array object.
/// @param additional The amount to increase the capacity by.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED @a additional_capacity is too big
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @success The capacity increased by at least the specified amount.
int dx_byte_array_increase_capacity(dx_byte_array* self, dx_size additional);

/// @brief Ensure the free capacity is greater than or equal to a specified value.
/// @param self A pointer to this dx_byte_array object.
/// @param required The required free capacity.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED @a required_free_capacity is too big
/// - #DX_ALLOCATION_FAILED an allocation failed
int dx_byte_array_ensure_free_capacity(dx_byte_array* self, dx_size required);

/// @brief Prepend Bytes.
/// @param self A pointer to this dx_byte_array object.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a bytes is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @success The Bytes in the array pointed to by @a bytes were prepended to this Byte buffer. 
int dx_byte_array_prepend(dx_byte_array* self, char const* bytes, dx_size number_of_bytes);

/// @brief Append Bytes.
/// @param self A pointer to this dx_byte_array object.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a bytes is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @success The Bytes in the array pointed to by @a bytes were appended to this Byte buffer. 
int dx_byte_array_append(dx_byte_array* self, char const* bytes, dx_size number_of_bytes);

/// @brief Insert Bytes.
/// @param self A pointer to this dx_byte_array object.
/// @param index The index in the Byte buffer.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a bytes is a null pointer
/// - #DX_INVALID_ARGUMENT @a index is greater than the size of this dx_byte_array object
/// - #DX_ALLOCATION_FAILED an allocation failed
/// @success The Bytes in the array pointed to by @a bytes were inserted at the specified index to this Byte buffer. 
int dx_byte_array_insert(dx_byte_array* self, dx_size index, char const* bytes, dx_size number_of_bytes);

/// @brief Get the size, in elements.
/// @param self A pointer to this dx_byte_array object.
/// @return The size on success. The size may be zero. The zero value is also returned on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size dx_byte_array_get_size(dx_byte_array const* self);

/// @brief Get a pointer to the Bytes.
/// @param self A pointer to this dx_byte_array.
/// @return A pointer to the Bytes on success. The null pointer on failure.
/// @remarks The pointer will invalidate if the Byte buffer is modified.
void* dx_byte_array_get_bytes(dx_byte_array* self);

/// @brief Get the capacity, in elements.
/// @param self A pointer to this dx_byte_array object.
/// @return The capacity on success. The capacity may be zero. The zero value is also returned on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size dx_byte_array_get_capacity(dx_byte_array const* self);

/// @brief Get the free capacity, in elements.
/// @param self A pointer to this dx_byte_array object.
/// @return The free capacity on success. The free capacity may be zero. The zero value is also returned on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
dx_size dx_byte_array_get_free_capacity(dx_byte_array const* self);

/// @brief Clear this Byte object.
/// @param self A pointer to this dx_byte_array object.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
int dx_byte_array_clear(dx_byte_array* self);

#endif // DX_CORE_BYTE_ARRAY_H_INCLUDED
