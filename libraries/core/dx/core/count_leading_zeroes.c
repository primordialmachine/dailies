#include "dx/core/count_leading_zeroes.h"

#if defined(_MSC_VER)
  #include <intrin.h>
#endif

dx_size dx_count_leading_zeroes_n8(dx_n8 x) {
  // Maps a value from 0000 (0x0) to 1111 (0xf) the value's number of leading bits.
  static const short lookup[16] = {
    4, //  0: 0000
    3, //  1: 0001
    2, //  2: 0010
    2, //  3: 0011
    1, //  4: 0100
    1, //  5: 0101
    1, //  6: 0110
    1, //  7: 0111
    0, //  8: 1000
    0, //  9: 1001
    0, // 10: 1010
    0, // 11: 1011
    0, // 12: 1100
    0, // 13: 1101
    0, // 14: 1110
    0, // 15: 1111
  };
  uint8_t upper = (x & 0xf0) >> 4;
  uint8_t lower = (x & 0x0f) >> 0;
  return upper ? lookup[upper] : 4 + lookup[lower];
}

dx_size dx_count_leading_zeroes_n16(dx_n16 x) {
  dx_n16 t;
  t = (x & 0xff00) >> 8;
  if (t) return dx_count_leading_zeroes_n8((dx_n8)t);
  t = (x & 0x00ff) >> 0;
  return 8 + dx_count_leading_zeroes_n8((dx_n8)t);  
}

dx_size dx_count_leading_zeroes_n32(dx_n32 x) {
#if defined(_MSC_VER)
  unsigned long n;
  if (_BitScanReverse(&n, x)) {
    // x was not zero.
    return (short)32 - ((short)n + 1);
  } else {
    // x was zero.
    return (short)32;
  }
#else
  dx_n32 t;
  t = (x & 0xffff0000) >> 16;
  if (!t) return dx_count_leading_zeroes_n16((dx_n16)t);
  t = (x & 0x0000ffff) >> 0;
  return 16 + dx_count_leading_zeroes_n16((dx_n16)t);
#endif
}

dx_size dx_count_leading_zeroes_n64(dx_n64 x) {
  uint32_t hi = (x & 0xffffffff00000000) >> 32;
  uint32_t lo = (x & 0x00000000ffffffff) >> 0;
  if (hi) {
    return dx_count_leading_zeroes_n32(hi);
  } else {
    return 32 + dx_count_leading_zeroes_n32(lo);
  }
}

dx_size dx_count_leading_zeroes_sz(dx_size x) {
#if defined(_M_X64)
  return dx_count_leading_zeroes_n64(x);
#elif defined(_M_IX86)
  return dx_count_leading_zeroes_n32(x);
#else
  #error("environment not supported")
#endif
}
