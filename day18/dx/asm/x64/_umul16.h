#if !defined(DX_ASM_X64__UMUL16_H_INCLUDED)
#define DX_ASM_X64__UMUL16_H_INCLUDED

#if defined(_M_X64)

#include <inttypes.h>

// compute the product of two uint8_t values
// store the high word of the uint16_t result in *z
// return the low word of the uint16_t result
uint8_t _dx_umul16(uint8_t x, uint8_t y, uint8_t* z);

#endif

#endif // DX_ASM_X64__UMUL16_H_INCLUDED
