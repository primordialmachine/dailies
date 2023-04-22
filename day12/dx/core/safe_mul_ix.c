#include "dx/core/safe_mul_ix.h"

#if defined(DX_MUL_I8_ASM) && 1 == DX_MUL_I8_ASM
  #if defined(_M_X64)
    #include "dx/asm/x64/_imul16.h"
  #elif defined(_M_IX86)
    #include "dx/asm/x86/_imul16.h"
  #else
    #error("environment not supported")
  #endif
#endif

int8_t dx_mul_i8(int8_t x, int8_t y, int8_t* z) {
#if defined(DX_MUL_I8_ASM) && 1 == DX_MUL_I8_ASM
  int8_t lo, hi;
  lo = _dx_imul16(x, y, &hi);
  *z = hi;
  return lo;
#else
  int16_t t = (int16_t)x * (int16_t)y;
  *z = (int8_t)((t & 0xff00) >> 8);
  return (int8_t)(t & 0x00ff);
#endif
}

#if defined(DX_MUL_I16_ASM) && 1 == DX_MUL_I16_ASM
  #if defined(_M_X64)
    #include "dx/asm/x64/_imul32.h"
  #elif defined(_M_IX86)
    #include "dx/asm/x86/_imul32.h"
  #else
    #error("environment not supported")
  #endif
#endif

int16_t dx_mul_i16(int16_t x, int16_t y, int16_t* z) {
#if defined(DX_MUL_I16_ASM) && 1 == DX_MUL_I16_ASM
  int16_t lo, hi;
  lo = _dx_imul32(x, y, &hi);
  *z = hi;
  return lo;
#else
  int32_t t = (int32_t)x * (int32_t)y;
  *z = (int16_t)((t & 0xffff0000) >> 16);
  return (int16_t)(t & 0x0000ffff);
#endif
}

#if defined(DX_MUL_I32_ASM) && 1 == DX_MUL_I32_ASM
  #if defined(_M_X64)
    #include "dx/asm/x64/_imul64.h"
  #elif defined(_M_IX86)
    #include "dx/asm/x86/_imul64.h"
  #else
    #error("environment not supported")
  #endif
#endif

int32_t dx_mul_i32(int32_t x, int32_t y, int32_t* z) {
#if defined(DX_MUL_I32_ASM) && 1 == DX_MUL_I32_ASM
  int32_t lo, hi;
  lo = _dx_imul64(x, y, &hi);
  *z = hi;
  return lo;
#else
  int64_t t = (int64_t)x * (int64_t)y;
  *z = (int32_t)((t & 0xffffffff00000000) >> 32);
  return (int32_t)(t & 0x00000000ffffffff);
#endif
}

#if defined(DX_MUL_I64_ASM) && 1 == DX_MUL_I64_ASM
  #if defined(_M_X64)
    #include "dx/asm/x64/_imul128.h"
  #elif defined(_M_IX86)
    #include "dx/asm/x86/_imul128.h"
  #else
    #error("environment not supported")
  #endif
#endif

int64_t dx_mul_i64(int64_t x, int64_t y, int64_t* z) {
#if defined(DX_MUL_I64_ASM) && 1 == DX_MUL_I64_ASM
  int64_t lo, hi;
  lo = _dx_imul128(x, y, &hi);
  *z = hi;
  return lo;
#else
  #error("environment not supported")
#endif
}

#if defined(DX_SAFE_MUL_IX_WITH_TESTS) && 1 == DX_SAFE_MUL_IX_WITH_TESTS

#define DEFINE(A, B, U, V) \
{ \
  dx_i8 u, v; \
  u = dx_mul_i8(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

static int dx_safe_mul_i8_tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(DX_I8_GREATEST, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, DX_I8_GREATEST, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(DX_I8_GREATEST, 1, DX_I8_GREATEST, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, DX_I8_GREATEST, DX_I8_GREATEST, 0);

  return 0;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  dx_i16 u, v; \
  u = dx_mul_i16(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

static int dx_safe_mul_i16_tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(DX_I16_GREATEST, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, DX_I16_GREATEST, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(DX_I16_GREATEST, 1, DX_I16_GREATEST, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, DX_I16_GREATEST, DX_I16_GREATEST, 0);

  return 0;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  dx_i32 u, v; \
  u = dx_mul_i32(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

static int dx_safe_mul_i32_tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(DX_I32_GREATEST, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, DX_I32_GREATEST, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(DX_I32_GREATEST, 1, DX_I32_GREATEST, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, DX_I32_GREATEST, DX_I32_GREATEST, 0);

  return 0;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  dx_i64 u, v; \
  u = dx_mul_i64(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

static int dx_safe_mul_i64_tests() {
  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(DX_I64_GREATEST, 0, 0, 0);
  // 0 * MAX = (MAX, 0)
  DEFINE(0, DX_I64_GREATEST, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(DX_I64_GREATEST, 1, DX_I64_GREATEST, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, DX_I64_GREATEST, DX_I64_GREATEST, 0);

  return 0;
}

#undef DEFINE

int dx_safe_mul_ix_tests() {
  if (dx_safe_mul_i8_tests()) {
    return 1;
  }
  if (dx_safe_mul_i16_tests()) {
    return 1;
  }
  if (dx_safe_mul_i32_tests()) {
    return 1;
  }
  if (dx_safe_mul_i64_tests()) {
    return 1;
  }
  return 0;
}

#endif // DX_SAFE_MUL_IX_WITH_TESTS
