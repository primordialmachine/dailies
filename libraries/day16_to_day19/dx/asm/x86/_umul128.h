#if !defined(DX_ASM_X86__UMUL128_H_INCLUDED)
#define DX_ASM_X86__UMUL128_H_INCLUDED

#if defined(_M_IX86)

#include <inttypes.h>

// compute the product of two uint64_t values
// store the high word of the uint128_t_t result in *z
// return the low word of the uint128_t result
uint64_t _dx_umul128(uint64_t multiplier, uint64_t multiplicand, uint64_t* product_hi);

#endif

#endif // DX_ASM_X86__UMUL128_H_INCLUDED
