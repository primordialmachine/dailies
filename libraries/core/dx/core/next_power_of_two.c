#include "dx/core/next_power_of_two.h"

#include "dx/core/count_leading_zeroes.h"

dx_n8 dx_next_power_of_two_n8(dx_n8 x) {
  if (x < 2) {
    return 1;
  }
  dx_size i = dx_count_leading_zeroes_n8(x);
  dx_n8 t = 1 << (8 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_OVERFLOW);
    return 0;
  }
  return t;
}

dx_n16 dx_next_power_of_two_n16(dx_n16 x) {
  if (x < 2) {
    return 1;
  }
  dx_size i = dx_count_leading_zeroes_n16(x);
  dx_n16 t = 1 << (16 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_OVERFLOW);
    return 0;
  }
  return t;
}

dx_n32 dx_next_power_of_two_n32(dx_n32 x) {
  if (x < 2) {
    return 1;
  }
  dx_size i = dx_count_leading_zeroes_n32(x);
  dx_n32 t = 1 << (32 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_OVERFLOW);
    return 0;
  }
  return t;
}

dx_n64 dx_next_power_of_two_n64(dx_n64 x) {
  if (x < 2) {
    return 1;
  }
  dx_size i = dx_count_leading_zeroes_n64(x);
  dx_n64 t = 1 << (64 - i);
  if (i == 0 && t != x) {
    dx_set_error(DX_OVERFLOW);
    return 0;
  }
  return t;
}

dx_size dx_next_power_of_two_sz(dx_size x) {
#if defined(_M_X64)
  return dx_next_power_of_two_n64(x);
#elif defined(_M_IX86)
  return dx_next_power_of_two_n64(x);
#else
  #error("environment not supported")
#endif
}
