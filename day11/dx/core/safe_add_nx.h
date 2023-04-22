#if !defined(DX_CORE_SAFE_ADD_NX_H_INCLUDED)
#define DX_CORE_SAFE_ADD_NX_H_INCLUDED

#include <stdint.h>
#include "dx/core/core.h"
#include "dx/configuration.h"

/// @ingroup core
/// Compute the sum of two <code>uint8_t</code> values and return the low and the high word of the sum separatly.
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param [out] c A pointer to a <code>uint8_t</variable>.
/// @return
/// The low word of the sum.
/// @success
/// The high word of the sum was stored in <code>*c</code>.
/// The low word of the sum was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>uint8_t</code> variable, the behavior of this function is undefined.
uint8_t dx_add_u8(uint8_t a, uint8_t b, uint8_t* c);

/// @ingroup core
/// Compute the sum of two <code>uint16_t</code> values and return the low and the high word of the result separatly.
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param [out] c A pointer to a <code>uint16_t</variable>.
/// @return
/// The low word of the sum.
/// @success
/// The high word of the sum was stored in <code>*c</code>.
/// The low word of the sum was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>uint16_t</code> variable, the behavior of this function is undefined.
uint16_t dx_add_u16(uint16_t a, uint16_t b, uint16_t* c);

/// @ingroup core
/// Compute the sum of two <code>uint32_t</code> values and return the low and the high word of the result separatly.
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param [out] c A pointer to a <code>uint32_t</variable>.
/// @return
/// The low word of the sum.
/// @success
/// The high word of the sum was stored in <code>*c</code>.
/// The low word of the sum was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>uint32_t</code> variable, the behavior of this function is undefined.
uint32_t dx_add_u32(uint32_t a, uint32_t b, uint32_t* c);

/// @ingroup core
/// Compute the sum of two <code>uint64_t</code> values and return the low and the high word of the result separatly.
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param [out] c A pointer to a <code>uint64_t</variable>.
/// @return
/// The low word of the sum.
/// @success
/// The high word of the sum was stored in <code>*c</code>.
/// The low word of the sum was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>uint64_t</code> variable, the behavior of this function is undefined.
uint64_t dx_add_u64(uint64_t a, uint64_t b, uint64_t* c);

/// @ingroup core
/// Compute the sum of two <code>size_t</code> values and return the low and the high word of the result separatly.
/// @param a The augend/first summand.
/// @param b The addend/second summand.
/// @param [out] c A pointer to a <code>size_t</variable>.
/// @return
/// The low word of the sum.
/// @success
/// The high word of the sum was stored in <code>*c</code>.
/// The low word of the sum was returned.
/// @failure
/// This function cannot fail.
/// @warning
/// If <code>c</code> does not point to a <code>size_t</code> variable, the behavior of this function is undefined.
size_t dx_add_sz(size_t a, size_t b, size_t* c);

#if defined(DX_SAFE_ADD_NX_WITH_TESTS) && 1 == DX_SAFE_ADD_NX_WITH_TESTS

int dx_safe_add_nx_tests();

#endif // DX_SAFE_ADD_NX_WITH_TESTS

#endif // DX_CORE_SAFE_ADD_NX_H_INCLUDED
