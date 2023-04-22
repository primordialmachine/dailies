#if !defined(DX_ASM_X86__UMUL16_H_INCLUDED)
#define DX_ASM_X86__UMUL16_H_INCLUDED

#include <inttypes.h>

#if defined(_M_IX86)

// compute the product of two uint8_t values
// store the high word of the uint16_t result in *z
// return the low word of the uint16_t result
uint8_t _fastcall _dx_umul16(uint8_t x, uint8_t y, uint8_t* z);

#endif

#endif // DX_ASM_X86__UMUL16_H_INCLUDED
