#if !defined(DX_ASM_X64__IMUL64_H_INCLUDED)
#define DX_ASM_X64__IMUL64_H_INCLUDED

#if defined(_M_X64)

#include <inttypes.h>

// compute the product of two int32_t values
// store the high word of the int64_t result in *z
// return the low word of the int64_t result
int32_t _dx_imul64(int32_t x, int32_t y, int32_t* z);

#endif

#endif // DX_ASM_X64__IMUL64_H_INCLUDED
