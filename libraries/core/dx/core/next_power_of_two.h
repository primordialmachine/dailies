#if !defined(DX_NEXT_POWER_OF_TWO_H_INCLUDED)
#define DX_NEXT_POWER_OF_TWO_H_INCLUDED

#include "dx/core/core.h"

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param value The value.
/// @return The next power of two greater than or equal to @a x on success. @a 0 on failure.
/// @default-failure
/// In particular, #DX_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_n8 dx_next_power_of_two_n8(dx_n8 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param value The value.
/// @return The next power of two greater than or equal to @a x on success. @a 0 on failure.
/// @default-failure
/// In particular, #DX_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_n16 dx_next_power_of_two_n16(dx_n16 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param value The value.
/// @return The next power of two greater than or equal to @a x on success. @a 0 on failure.
/// @default-failure
/// In particular, #DX_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_n32 dx_next_power_of_two_n32(dx_n32 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param value The value.
/// @return The next power of two greater than or equal to @a x on success. @a 0 on failure.
/// @default-failure
/// In particular, #DX_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_n64 dx_next_power_of_two_n64(dx_n64 x);

/// @brief Get the next power of two greater than or equal to the specified value.
/// @param value The value.
/// @return The next power of two greater than or equal to @a x on success. @a 0 on failure.
/// @default-failure
/// In particular, #DX_OVERFLOW if there is no representable next power of 2 greater than or equal to @a x.
dx_size dx_next_power_of_two_sz(dx_size x);

#endif // DX_NEXT_POWER_OF_TWO_H_INCLUDED

