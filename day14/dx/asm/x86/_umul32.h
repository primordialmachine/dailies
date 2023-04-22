#if !defined(DX_ASM_X86__UMUL32_H_INCLUDED)
#define DX_ASM_X86__UMUL32_H_INCLUDED

#include <inttypes.h>

#if defined(_M_IX86)

// compute the product of two uint16_t values
// store the high word of the uint32_t result in *z
// return the low word of the uint32_t result
uint16_t _fastcall _dx_umul32(uint16_t x, uint16_t y, uint16_t* z);

#endif

#endif // DX_ASM_X86__UMUL32_H_INCLUDED
