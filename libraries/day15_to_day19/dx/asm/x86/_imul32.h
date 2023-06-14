#if !defined(DX_ASM_X86__IMUL32_H_INCLUDED)
#define DX_ASM_X86__IMUL32_H_INCLUDED

#include <inttypes.h>

#if defined(_M_IX86)

// compute the product of two int16_t values
// store the high word of the int32_t result in *z
// return the low word of the int32_t result
int16_t _fastcall _dx_imul32(int16_t x, int16_t y, int16_t* z);

#endif

#endif // DX_ASM_X86__IMUL32_H_INCLUDED
