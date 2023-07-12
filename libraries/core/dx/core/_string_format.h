#if !defined(DX_CORE__STRING_FORMAT_H_INCLUDED)
#define DX_CORE__STRING_FORMAT_H_INCLUDED

#include "dx/core/byte_array.h"
#include <stdarg.h>

int dx__format_v(dx_byte_array* buffer, char const* start, char const* end, va_list arguments);

#endif // DX_CORE__STRING_FORMAT_H_INCLUDED
