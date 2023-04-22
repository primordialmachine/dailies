#if !defined(DX_COUNT_LEADING_ZEROES_H_INCLUDED)
#define DX_COUNT_LEADING_ZEROES_H_INCLUDED

#include "dx/core/core.h"

/// @brief Count the leading zeroes of the binary representation of the specified value.
/// @param x The value.
/// @return The leading zeroes.
dx_size dx_count_leading_zeroes_n8(dx_n8 x);

/// @brief Count the leading zeroes of the binary representation of the specified value.
/// @param x The value.
/// @return The leading zeroes.
dx_size dx_count_leading_zeroes_n16(dx_n16 x);

/// @brief Count the leading zeroes of the binary representation of the specified value.
/// @param x The value.
/// @return The leading zeroes.
dx_size dx_count_leading_zeroes_n32(dx_n32 x);

/// @brief Count the leading zeroes of the binary representation of the specified value.
/// @param x The value.
/// @return The leading zeroes.
dx_size dx_count_leading_zeroes_n64(dx_n64 x);

/// @brief Count the leading zeroes of the binary representation of the specified value.
/// @param x The value.
/// @return The leading zeroes.
dx_size dx_count_leading_zeroes_sz(dx_size x);

#endif // DX_COUNT_LEADING_ZEROES_H_INCLUDED
