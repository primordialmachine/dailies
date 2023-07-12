#if !defined(DX_CORE_STRING_H_INCLUDED)
#define DX_CORE_STRING_H_INCLUDED

#include "dx/core/object.h"

/// @brief The type of a string.
typedef struct dx_string dx_string;
static inline dx_string* DX_STRING(void* p) {
  return (dx_string*)p;
}

/// @details
/// The following format specifiers are currently supported:
/// - `${s}` a pointer to a dx_string object. Prints the contents of this string.
/// - `$$` prints `%`.
/// - `${i8}` a dx_i8 value.
/// - `${i16}` a dx_i16 value.
/// - `${i32}` a dx_i32 value.
/// - `${i64}` a dx_i64 value.
/// - `${n8}` a dx_n8 value.
/// - `${n16}` a dx_n16 value.
/// - `${n32}` a dx_n32 value.
/// - `${n64}` a dx_n8 value.
/// - `${f32}` a dx_f32 value.
/// - `${f64}` a dx_f64 value.
dx_string* dx_string_printfv(dx_string* format, va_list arguments);

dx_string* dx_string_printf(dx_string* format, ...);

/// @brief Create a string from an array of Bytes representing a value UTF-8 string.
/// @param bytes A pointer to an array of @a number_of_bytes Bytes.
/// @param number_of_bytes The number of Bytes in the array pointed to by @a bytes.
/// @return A pointer to the string on success. A null pointer on failure.
/// @default-failure
dx_string* dx_string_create(char const* p, dx_size n);

/// @brief Get a pointer to the Bytes of this string.
/// @param self A pointer to this string.
/// @return A pointer to the Bytes of the string on success. A null pointer on failure.
/// @default-failure
void const* dx_string_get_bytes(dx_string const* self);

/// @brief Get the number of Bytes of this string.
/// @param self A pointer to this string.
/// @return The number of Bytes of this string. This number might be @a 0.
/// @a 0 is also returned if an error occurred. 
/// @default-failure
dx_size dx_string_get_number_of_bytes(dx_string const* self);

/// @brief Get if the string contains an UTF-8 symbol.
/// @param self A pointer to this string.
/// @param symbol The UTF-8 symbol.
/// @return @a true if the string contains the symbol. @a false if the string does not contain the symbol or an error occurred.
/// @default-failure
bool dx_string_contains_symbol(dx_string const* self, uint32_t symbol);

/// @brief Get if this string is equal to another string.
/// @param self A pointer to this string.
/// @param other A pointer to the other string.
/// @return @a true if this string is equal to the other string. @a false otherwise. @a false is also returned on failure.
/// @default-failure
bool dx_string_is_equal_to(dx_string const* self, dx_string const* other);

/// @brief Get the hash value of this string.
/// @param self A pointer to this string.
/// @return The hash value of this string. This may be @a 0. @a 0 is also returned on failure.
/// @default-failure
dx_size dx_string_get_hash_value(dx_string const* self);

struct dx_string {
  dx_object _parent;
  dx_size number_of_bytes;
  char bytes[];
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// Convert a string into an int64_t value.
/// @param p
/// A pointer to the string.
/// @param l
/// The length, in Bytes, of the string.
/// @param [out] v
/// Pointer to the variable receiving the value.
/// @return
/// The zero value on success. A non-zero value on failure.
/// @failure
/// This function has set the error variable.
/// @success
/// <code>*v</code> was assigned the value.
int dx_str_to_i64(char const* p, dx_size l, int64_t* v);

/// @brief
/// Convert a string into an uint64_t value.
/// @param p
/// A pointer to a string.
/// @param l
/// The length, in Bytes, of the string.
/// @param [out] v
/// Pointer to the variable receiving the value.
/// @return
/// The zero value on success. A non-zero value on failure.
/// @failure
/// This function has set the error variable.
/// @success
/// <code>*v</code> was assigned the value.
int dx_str_to_u64(char const* p, dx_size l, uint64_t* v);

#endif // DX_CORE_STRING_H_INCLUDED
