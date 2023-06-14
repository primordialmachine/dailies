#if !defined(DX_ASM_X64__IMUL16_H_INCLUDED)
#define DX_ASM_X64__IMUL16_H_INCLUDED

#if defined(_M_X64)

#include <inttypes.h>

// compute the product of two int8_t values
// store the high word of the int16_t result in *z
// return the low word of the int16_t result
int8_t _dx_imul16(int8_t x, int8_t y, int8_t* z);

#endif

#endif // DX_ASM_X64__IMUL16_H_INCLUDED
