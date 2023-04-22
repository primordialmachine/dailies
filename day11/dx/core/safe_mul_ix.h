#if !defined(DX_CORE_SAFE_MUL_IX_H_INCLUDED)
#define DX_CORE_SAFE_MUL_IX_H_INCLUDED

#include "dx/core/core.h"
#include "dx/configuration.h"

/// @ingroup core
/// Compute the product of two <code>int8_t</code> values and return the low and the high word of the result separatly.
/// @param x The multiplier/first factor.
/// @param y The multiplicand/second factor.
/// @param [out] c A pointer to a <code>int8_t</variable>.
/// @return
/// The low word of the product.
/// @success
/// The high word of the product was stored in <code>*c</code>.
/// The low word of the product was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>int8_t</code> variable, the behavior of this function is undefined.
int8_t dx_mul_i8(int8_t x, int8_t y, int8_t* z);

/// @ingroup core
/// Compute the product of two <code>int16_t</code> values and return the low and the high word of the result separatly.
/// @param x The multiplier/first factor.
/// @param y The multiplicand/second factor.
/// @param [out] z A pointer to a <code>int16_t</variable>.
/// @return
/// The low word of the product.
/// @success
/// The high word of the product was stored in <code>*c</code>.
/// The low word of the product was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>int16_t</code> variable, the behavior of this function is undefined.
int16_t dx_mul_i16(int16_t x, int16_t y, int16_t* z);

/// @ingroup core
/// Compute the product of two <code>int32_t</code> values and return the low and the high word of the result separatly.
/// @param x The multiplier/first factor.
/// @param y The multiplicand/second factor.
/// @param [out] z A pointer to a <code>int32_t</variable>.
/// @return
/// The low word of the product.
/// @success
/// The high word of the product was stored in <code>*c</code>.
/// The low word of the product was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>int32_t</code> variable, the behavior of this function is undefined.
int32_t dx_mul_i32(int32_t x, int32_t y, int32_t* z);

/// @ingroup core
/// Compute the product of two <code>int64_t</code> values and return the low and the high word of the result separatly.
/// @param x The multiplier/first factor.
/// @param y The multiplicand/second factor.
/// @param [out] z A pointer to a <code>int64_t</variable>.
/// @return
/// The low word of the product.
/// @success
/// The high word of the product was stored in <code>*c</code>.
/// The low word of the product was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>int64_t</code> variable, the behavior of this function is undefined.
int64_t dx_mul_i64(int64_t x, int64_t y, int64_t* z);

#if defined(DX_SAFE_MUL_IX_WITH_TESTS) && 1 == DX_SAFE_MUL_IX_WITH_TESTS

int dx_safe_mul_ix_tests();

#endif // DX_SAFE_MUL_IX_WITH_TESTS

#endif // DX_CORE_SAFE_MUL_IX_H_INCLUDED
