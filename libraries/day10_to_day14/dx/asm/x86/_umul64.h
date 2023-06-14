#if !defined(DX_ASM_X86__UMUL64_H_INCLUDED)
#define DX_ASM_X86__UMUL64_H_INCLUDED

#include <inttypes.h>

#if defined(_M_IX86)

// compute the product of two uint32_t values
// store the high word of the uint64_t result in *z
// return the low word of the uint64_t result
uint32_t _fastcall _dx_umul64(uint32_t x, uint32_t y, uint32_t* z);

#endif

#endif // DX_ASM_X86__UMUL64_H_INCLUDED
