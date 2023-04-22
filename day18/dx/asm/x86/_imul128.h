#if !defined(DX_ASM_X86__IMUL128_H_INCLUDED)
#define DX_ASM_X86__IMUL128_H_INCLUDED

#if defined(_M_IX86)

#include <inttypes.h>

// compute the product of two int64_t values
// store the high word of the int128_t_t result in *z
// return the low word of the int128_t result
int64_t _dx_imul128(int64_t x, int64_t y, int64_t* z);

#endif // 

#endif // DX_ASM_X86__IMUL128_H_INCLUDED
