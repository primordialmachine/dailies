#include "dx/core/safe_mul_nx.h"

#include "dx/configuration.h"

#if defined(DX_MUL_N8_ASM) && 1 == DX_MUL_N8_ASM
  #if defined(_M_X64)
    #include "dx/asm/x64/_umul16.h"
  #elif defined(_M_IX86)
    #include "dx/asm/x86/_umul16.h"
  #else
    #error("environment not supported")
  #endif
#endif

uint8_t dx_mul_u8(uint8_t x, uint8_t y, uint8_t* z) {
#if defined(DX_MUL_N8_ASM) && 1 == DX_MUL_N8_ASM
  uint8_t lo, hi;
  lo = _dx_umul16(x, y, &hi);
  *z = hi;
  return lo;
#else
  uint16_t t = (uint16_t)x * (uint16_t)y;
  *z = (uint8_t)((t & 0xff00) >> 8);
  return (uint8_t)(t & 0x00ff);
#endif
}

#if defined(DX_MUL_N16_ASM) && 1 == DX_MUL_N16_ASM
  #if defined(_M_X64)
    #include "dx/asm/x64/_umul32.h"
  #elif defined(_M_IX86)
    #include "dx/asm/x86/_umul32.h"
  #else
      #error("environment not supported")
  #endif
#endif

uint16_t dx_mul_u16(uint16_t x, uint16_t y, uint16_t* z) {
#if defined(DX_MUL_N16_ASM) && 1 == DX_MUL_N16_ASM
  uint16_t lo, hi;
  lo = _dx_umul32(x, y, &hi);
  *z = hi;
  return lo;
#else
  uint32_t t = (uint32_t)x * (uint32_t)y;
  *z = (uint16_t)((t & 0xffff0000) >> 16);
  return (uint16_t)(t & 0x0000ffff);
#endif
}

#if defined(DX_MUL_N32_ASM) && 1 == DX_MUL_N32_ASM
  #if defined(_M_X64)
    #include "dx/asm/x64/_umul64.h"
  #elif defined(_M_IX86)
    #include "dx/asm/x86/_umul64.h"
  #else
    #error("environment not supported")
  #endif
#endif

uint32_t dx_mul_u32(uint32_t x, uint32_t y, uint32_t* z) {
#if defined(DX_MUL_N32_ASM) && 1 == DX_MUL_N32_ASM
  uint32_t lo, hi;
  lo = _dx_umul64(x, y, &hi);
  *z = hi;
  return lo;
#else
  uint64_t t = (uint64_t)x * (uint64_t)y;
  *z = (uint32_t)((t & 0xffffffff00000000) >> 32);
  return (uint32_t)(t & 0x00000000ffffffff);
#endif
}

#if defined(_M_X64)
  #include "dx/asm/x64/_umul128.h"
#elif defined(_M_IX86)
  #include "dx/asm/x86/_umul128.h"
#else
  #error("environment not supported")
#endif

uint64_t dx_mul_u64(uint64_t x, uint64_t y, uint64_t* z) {
  uint64_t lo, hi;
  lo = _dx_umul128(x, y, &hi);
  *z = hi;
  return lo;
}

size_t dx_mul_sz(size_t a, size_t b, size_t* c) {
#if defined(_M_X64)
  return dx_mul_u64(a, b, c);
#elif defined(_M_IX86)
  return dx_mul_u32(a, b, c);
#else
  #error("environment not supported")
#endif
}

#if defined(DX_SAFE_MUL_NX_WITH_TESTS) && 1 == DX_SAFE_MUL_NX_WITH_TESTS

/// @{
/// @internal
/// @brief Get the @a n most significant bits a dx_nx value.
/// @param x The dx_nx value.
/// @param n The @a n bits to get. The behavior is undefined if @a n is greater than @a DX_NX_NUMBER_OF_BITS. 
/// @return
/// Let <code>n</code> be the number of bits to get.
/// If <code>n</code> is @a 0, then @a 0 is returned.
/// Let <code>sum_{i=0}^DX_NUMBER_OF_BITS 2^i * x[i]</code> be the binary representation of @a x.
/// Then the value <code>sum_{i=DX_NX_NUMBER_OF_BITS-n}^DX_NX_NUMBER_OF_BITS 2^i * x[i] / 2^(DX_NX_NUMBER_OF_BITS-n)</code> is returned..
/// 
/// By the above, let's take a look at some examples for <code>DX_N8_NUMBER_OF_BITS</code>.
/// - @code{n = 0}.
/// By the above, @a 0 is returned.
/// 
/// - @code{n = 1}
/// By the above, <code>sum_{i=7}^7 2^i x_i / 2^7 = 2^7 x_7 / 2^7 = 2^0 x_7</code> is returned.
/// 
/// - @code{n = 2}
/// By the above <code>sum_{i=6}^7 2^i x_i / 2^6 = (2^6 x_6 + 2^7 x_7) / 2^6 = 2^0 x_6 + 2^1 x_7</code> is returned.
/// 
/// - @code{n = 8}
/// By the above <code>sum_{i=0}^7 2^i x_i / 2^0 = \sum_{i=0}^7 2^i x_i = x</code> is returned.
/// 
static dx_n8 get_msbs_n8(dx_n8 x, dx_n8 n) {
  dx_n8 mask = 0;
  for (size_t i = 8; i > 8 - n; --i) {
    mask |= ((dx_n8)1) << (i - 1);
  }
  return (x & mask) >> (8 - n);
}

static dx_n16 get_msbs_n16(dx_n16 x, dx_n8 n) {
  dx_n16 mask = 0;
  for (size_t i = 16; i > 16 - n; --i) {
    mask |= ((dx_n16)1) << (i - 1);
  }
  return (x & mask) >> (16 - n);
}

static dx_n32 get_msbs_n32(dx_n32 x, dx_n8 n) {
  dx_n32 mask = 0;
  for (size_t i = 32; i > 32 - n; --i) {
    mask |= ((dx_n32)1) << (i - 1);
  }
  return (x & mask) >> (32 - n);
}

static dx_n64 get_msbs_n64(dx_n64 x, dx_n8 n) {
  dx_n64 mask = 0;
  for (size_t i = 64; i > 64 - n; --i) {
    mask |= ((dx_n64)1) << (i - 1);
  }
  return (x & mask) >> (64 - n);
}

#define DEFINE(A, B, U, V) \
{ \
  dx_n8 u, v; \
  u = dx_mul_u8(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}


int dx_safe_mul_n8_tests() {
  dx_n8 u, v;

  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);

  // MAX * 0 = (MAX, 0)
  DEFINE(DX_N8_GREATEST, 0, 0, 0);
  
  // 0 * MAX = (MAX, 0)
  u = dx_mul_u8(0, DX_N8_GREATEST, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  
  // MAX * 1 = (MAX, 0)
  u = dx_mul_u8(DX_N8_GREATEST, 1, &v);
  if (u != DX_N8_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // 1 * MAX = (MAX, 0)
  u = dx_mul_u8(1, DX_N8_GREATEST, &v);
  if (u != DX_N8_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  uint8_t a, b;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint8_t)(DX_N8_GREATEST << 1);
  b = get_msbs_n8(DX_N8_GREATEST, 1);

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u8(DX_N8_GREATEST, 2, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u8(2, DX_N8_GREATEST, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint8_t)(DX_N8_GREATEST << 2);
  b = get_msbs_n8(DX_N8_GREATEST, 2);

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u8(DX_N8_GREATEST, 4, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u8(4, DX_N8_GREATEST, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  return 0;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  dx_n16 u, v; \
  u = dx_mul_u16(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

int dx_safe_mul_n16_tests() {
  dx_n16 u, v;

  // 0 * 0 = (0, 0)
  DEFINE(0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  DEFINE(DX_N16_GREATEST, 0, 0, 0);
  // 0 * MAX = (0, 0)
  DEFINE(0, DX_N16_GREATEST, 0, 0);
  // MAX * 1 = (MAX, 0)
  DEFINE(DX_N16_GREATEST, 1, DX_N16_GREATEST, 0);
  // 1 * MAX = (MAX, 0)
  DEFINE(1, DX_N16_GREATEST, DX_N16_GREATEST, 0);

  uint16_t a, b;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint16_t)(DX_N16_GREATEST << 1);
  b = get_msbs_n16(DX_N16_GREATEST, 1);

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u16(DX_N16_GREATEST, 2, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u16(2, DX_N16_GREATEST, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint16_t)(DX_N16_GREATEST << 2);
  b = get_msbs_n16(DX_N16_GREATEST, 2);

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u16(DX_N16_GREATEST, 4, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u16(4, DX_N16_GREATEST, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  return 0;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  dx_n32 u, v; \
  u = dx_mul_n32(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

int dx_safe_mul_n32_tests() {
  dx_n32 u, v;

  // 0 * 0 = (0, 0)
  u = dx_mul_u32(0, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // MAX * 0 = (MAX, 0)
  u = dx_mul_u32(DX_N32_GREATEST, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  
  // 0 * MAX = (MAX, 0)
  u = dx_mul_u32(0, DX_N32_GREATEST, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // MAX * 1 = (MAX, 0)
  u = dx_mul_u32(DX_N32_GREATEST, 1, &v);
  if (u != DX_N32_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  
  // 1 * MAX = (MAX, 0)
  u = dx_mul_u32(1, DX_N32_GREATEST, &v);
  if (u != DX_N32_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  uint32_t a, b;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint32_t)(DX_N32_GREATEST << 1);
  b = get_msbs_n32(DX_N32_GREATEST, 1);

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u32(DX_N32_GREATEST, 2, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u32(2, DX_N32_GREATEST, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint32_t)(DX_N32_GREATEST << 2);
  b = get_msbs_n32(DX_N32_GREATEST, 2);

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u32(DX_N32_GREATEST, 4, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u32(4, DX_N32_GREATEST, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  return 0;
}

#undef DEFINE

#define DEFINE(A, B, U, V) \
{ \
  dx_n64 u, v; \
  u = dx_mul_n64(A, B, &v); \
  if (u != U) { \
    return 1; \
  } \
  if (v != V) { \
    return 1; \
  } \
}

int dx_safe_mul_n64_tests() {
  dx_n64 u, v;

  // 0 * 0 = (0, 0)
  u = dx_mul_u64(0, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // MAX * 0 = (MAX, 0)
  u = dx_mul_u64(DX_N64_GREATEST, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // 0 * MAX = (MAX, 0)
  u = dx_mul_u64(0, DX_N64_GREATEST, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // MAX * 1 = (MAX, 0)
  u = dx_mul_u64(DX_N64_GREATEST, 1, &v);
  if (u != DX_N64_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  // 1 * MAX = (MAX, 0)
  u = dx_mul_u64(1, DX_N64_GREATEST, &v);
  if (u != DX_N64_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }

  uint64_t a, b;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint64_t)(DX_N64_GREATEST << 1);
  b = get_msbs_n64(DX_N64_GREATEST, 1);

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u64(DX_N64_GREATEST, 2, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 2 = (lsift1(MAX), msb1(MAX))
  u = dx_mul_u64(2, DX_N64_GREATEST, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  a = (uint64_t)(DX_N64_GREATEST << 2);
  b = get_msbs_n64(DX_N64_GREATEST, 2);

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u64(DX_N64_GREATEST, 4, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  // MAX * 4 = (lsift2(MAX), msb2(MAX))
  u = dx_mul_u64(4, DX_N64_GREATEST, &v);
  if (u != a) {
    return 1;
  }
  if (v != b) {
    return 1;
  }

  return 0;
}

#undef DEFINE

int dx_safe_mul_nx_tests() {
  if (dx_safe_mul_n8_tests()) {
    return 1;
  }
  if (dx_safe_mul_n16_tests()) {
    return 1;
  }
  if (dx_safe_mul_n32_tests()) {
    return 1;
  }
  if (dx_safe_mul_n64_tests()) {
    return 1;
  }
  return 0;
}

#endif // DX_SAFE_MUL_NX_WITH_TESTS
