#if !defined(DX_ASM_X86__IMUL64_H_INCLUDED)
#define DX_ASM_X86__IMUL64_H_INCLUDED

#include <inttypes.h>

#if defined(_M_IX86)

// compute the product of two int32_t values
// store the high word of the int64_t result in *z
// return the low word of the int64_t result
int32_t _fastcall _dx_imul64(int32_t x, int32_t y, int32_t* z);

#endif

#endif // DX_ASM_X86__IMUL64_H_INCLUDED
