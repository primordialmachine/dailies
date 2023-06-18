#if !defined(DX_ASM_MUL_NX_H_INCLUDED)
#define DX_ASM_MUL_NX_H_INCLUDED

#include "dx/core/core.h"
#include "dx/core/configuration.h"

/// @ingroup core
/// Compute the product of two <code>uint8_t</code> values and return the low and the high word of the product separatly.
/// @param a The multiplier/first factor.
/// @param b The multiplicand/second factor.
/// @param [out] c A pointer to a <code>uint8_t</code> variable.
/// @return
/// The low word of the product.
/// @success
/// The high word of the product was stored in <code>*c</code>.
/// The low word of the product was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>uint8_t</code> variable, the behavior of this function is undefined.
uint8_t dx_mul_u8(uint8_t x, uint8_t y, uint8_t* z);

/// @ingroup core
/// Compute the product of two <code>uint16_t</code> values and return the low and the high word of the result separatly.
/// @param a The multiplier/first factor.
/// @param b The multiplicand/second factor.
/// @param [out] c A pointer to a <code>uint16_t</code> variable.
/// @return
/// The low word of the product.
/// @success
/// The high word of the product was stored in <code>*c</code>.
/// The low word of the product was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>uint16_t</code> variable, the behavior of this function is undefined.
uint16_t dx_mul_u16(uint16_t x, uint16_t y, uint16_t* z);

/// @ingroup core
/// Compute the product of two <code>uint32_t</code> values and return the low and the high word of the result separatly.
/// @param a The multiplier/first factor.
/// @param b The multiplicand/second factor.
/// @param [out] c A pointer to a <code>uint32_t</code> variable.
/// @return
/// The low word of the product.
/// @success
/// The high word of the product was stored in <code>*c</code>.
/// The low word of the product was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>uint32_t</code> variable, the behavior of this function is undefined.
uint32_t dx_mul_u32(uint32_t x, uint32_t y, uint32_t* z);

/// @ingroup core
/// Compute the product of two <code>uint64_t</code> values and return the low and the high word of the result separatly.
/// @param a The multiplier/first factor.
/// @param b The multiplicand/second factor.
/// @param [out] c A pointer to a <code>uint64_t</code> variable.
/// @return
/// The low word of the product.
/// @success
/// The high word of the product was stored in <code>*c</code>.
/// The low word of the product was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>uint64_t</code> variable, the behavior of this function is undefined.
uint64_t dx_mul_u64(uint64_t x, uint64_t y, uint64_t* z);

/// @ingroup core
/// Compute the product of two <code>size_t</code> values and return the low and the high word of the result separatly.
/// @param a The multiplier/first factor.
/// @param b The multiplicand/second factor.
/// @param [out] c A pointer to a <code>size_t</code> variable.
/// @return
/// The low word of the product.
/// @success
/// The high word of the product was stored in <code>*c</code>.
/// The low word of the product was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>size_t</code> variable, the behavior of this function is undefined.
size_t dx_mul_sz(size_t a, size_t b, size_t* c);

#if defined(DX_SAFE_MUL_NX_WITH_TESTS) && 1 == DX_SAFE_MUL_NX_WITH_TESTS

int dx_safe_mul_nx_tests();

#endif // DX_SAFE_MUL_NX_WITH_TESTS

#endif // DX_ASM_MUL_NX_H_INCLUDED
