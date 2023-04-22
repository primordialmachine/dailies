#include "dx/asm/x64/_umul128.h"

#include <intrin.h>

uint64_t _dx_umul128(uint64_t x, uint64_t y, uint64_t* z) {
  return _umul128(x, y, z);
}
