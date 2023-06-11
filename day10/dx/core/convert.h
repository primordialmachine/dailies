#if !defined(DX_CORE_CONVERT_H_INCLUDED)
#define DX_CORE_CONVERT_H_INCLUDED

#include "dx/core/core.h"

/// @ingroup core
/// @brief Convert a string to a <code>dx_i8</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_i8</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_i8(char const* p, dx_size n, dx_i8* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_n8</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_n8</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_n8(char const* p, dx_size n, dx_n8* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_i16</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_i16</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_i16(char const* p, dx_size n, dx_i16* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_n16</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_n16</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_n16(char const* p, dx_size n, dx_n16* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_i32</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_i32</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_i32(char const* p, dx_size n, dx_i32* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_n32</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_n32</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_n32(char const* p, dx_size n, dx_n32* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_i64</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_i64</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_i64(char const* p, dx_size n, dx_i64* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_n64</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_n64</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_n64(char const* p, dx_size n, dx_n64* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_size</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_size</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_sz(char const* p, dx_size n, dx_size* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_f32</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_f32</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_f32(char const* p, dx_size n, dx_f32* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_f64</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_f64</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_uf8bytes_to_f64(char const* p, dx_size n, dx_f64* target);

/// @ingroup core
/// @brief Convert a string to a <code>dx_bool</code> value.
/// @param p A pointer to an UTF-8 string of @a n Bytes.
/// @param n The length, in Bytes, of the UTF-8 string pointed to by @a p.
/// @param target A pointer to a <code>dx_bool</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_convert_utf8bytes_to_bool(char const* p, dx_size n, dx_bool* target);

#endif // DX_CORE_CONVERT_H_INCLUDED
