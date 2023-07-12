#include "dx/core/_string_format.h"

// memcpy
#include <string.h>
// snprintf
#include <stdio.h>

#include "dx/core/string.h"
//#include "dx/core/byte_array.h"
//#include "dx/core/memory.h"
//#include "dx/core/safe_add_nx.h"
//#include "dx/core/safe_mul_nx.h"
//#include <stdarg.h>

#define PRINT_PRECISION_8 (8)
#define PRINT_PRECISION_16 (16)
#define PRINT_PRECISION_32 (32)
#define PRINT_PRECISION_64 (64)

#define PRINT_ERROR (0)

#define PRINT_TYPE_I (1)
#define PRINT_TYPE_N (2)
#define PRINT_TYPE_S (3)
#define PRINT_TYPE_F (4)

#define PRINT_F32 (PRINT_TYPE_F | PRINT_PRECISION_32)
#define PRINT_F64 (PRINT_TYPE_F | PRINT_PRECISION_64)

#define PRINT_I16 (PRINT_TYPE_I | PRINT_PRECISION_16)
#define PRINT_I32 (PRINT_TYPE_I | PRINT_PRECISION_32)
#define PRINT_I64 (PRINT_TYPE_I | PRINT_PRECISION_64)
#define PRINT_I8  (PRINT_TYPE_I | PRINT_PRECISION_8)

#define PRINT_N16 (PRINT_TYPE_N | PRINT_PRECISION_16)
#define PRINT_N32 (PRINT_TYPE_N | PRINT_PRECISION_32)
#define PRINT_N64 (PRINT_TYPE_N | PRINT_PRECISION_64)
#define PRINT_N8  (PRINT_TYPE_N | PRINT_PRECISION_8)

#define PRINT_STRING (1024)
#define PRINT_DOLLAR (1025)

static bool _is_digit(char x) {
  return '0' <= x && x <= '9';
}

#define IS(SYMBOL) ((**start) == SYMBOL)

static int parse_precision(char const** start, char const* end) {
  if ((*start) == end)
    return PRINT_ERROR;
  
  if (IS('8')) {
    return PRINT_PRECISION_8;
  } else if ((**start) == '1') {
    (*start)++;
    if ((*start) == end)
      return PRINT_ERROR;
    if (!IS('6'))
      return PRINT_ERROR;
    return PRINT_PRECISION_16;
  } else if (IS('3')) {
    (*start)++;
    if ((*start) == end)
      return PRINT_ERROR;
    if (!IS('2'))
      return PRINT_ERROR;
    return PRINT_PRECISION_32;
  } else if (IS('6')) {
    (*start)++;
    if ((*start) == end)
      return PRINT_ERROR;
    if (!IS('4'))
      return PRINT_ERROR;
    return PRINT_PRECISION_64;
  } else {
    return PRINT_ERROR;
  }
}

static int parse(char const** start, char const* end) {
  assert(IS('$'));
  
  (*start)++;
  if ((*start) == end) return PRINT_ERROR;
  
  if (IS('$'))
    return PRINT_DOLLAR;
  if (!IS('{'))
    return PRINT_ERROR;

  (*start)++;
  if ((*start) == end)
    return PRINT_ERROR;
  if (IS('i')) {
    (*start)++;
    int k;
    int precision = parse_precision(start, end);
    if (precision == PRINT_ERROR)
      return PRINT_ERROR;
    k = PRINT_TYPE_I | precision;
    (*start)++;
    if ((*start) == end) return PRINT_ERROR;
    if (!IS('}')) return PRINT_ERROR;
    (*start)++;
    return k;
  } else if (IS('n')) {
    (*start)++;
    int k;
    int precision = parse_precision(start, end);
    if (precision == PRINT_ERROR)
      return PRINT_ERROR;
    k = PRINT_TYPE_N | precision;
    (*start)++;
    if ((*start) == end) return PRINT_ERROR;
    if (!IS('}')) return PRINT_ERROR;
    (*start)++;
    return k;
  } else if (IS('f')) {
    (*start)++;
    int k;
    int precision = parse_precision(start, end);
    if (precision != PRINT_PRECISION_32 && precision != PRINT_PRECISION_64)
      return PRINT_ERROR;
    k = PRINT_TYPE_F | precision;
    (*start)++;
    if ((*start) == end) return PRINT_ERROR;
    if (!IS('}')) return PRINT_ERROR;
    (*start)++;
    return k;
  } else if (IS('s')) {
    (*start)++;
    if ((*start) == end) return PRINT_ERROR;
    if (!IS('}')) return PRINT_ERROR;
    (*start)++;
    return PRINT_STRING;
  } else {
    return PRINT_ERROR;
  }
}

#undef IS

int dx__format_v(dx_byte_array* buffer, char const* start, char const* end, va_list arguments) {
  char const* current = start;
  while (current != end) {
    if (*current == '$') {
      // We encountered a format symbol. Store all the bytes up to and excluding the format symbol in the buffer.
      if (dx_byte_array_append(buffer, start, current - start)) {
        return 1;
      }
      int k = parse(&current, end);
      if (k == PRINT_ERROR) {
        dx_set_error(DX_INVALID_ARGUMENT);
        return 1;
      }
      switch (k) {
      case PRINT_STRING: {
        dx_string* argument = va_arg(arguments, dx_string*);
        if (!argument) {
          dx_set_error(DX_INVALID_ARGUMENT);
          return 1;
        }
        if (dx_byte_array_append(buffer, argument->bytes, argument->number_of_bytes)) {
          return 1;
        }
      } break;
      case PRINT_I8: {
        dx_i8 argument = va_arg(arguments, dx_i8);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIi8, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_I16: {
        dx_i16 argument = va_arg(arguments, dx_i16);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIi16, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_I32: {
        dx_i32 argument = va_arg(arguments, dx_i32);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIi32, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_I64: {
        dx_i64 argument = va_arg(arguments, dx_i64);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIi64, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_N8: {
        dx_n8 argument = va_arg(arguments, dx_n8);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIu8, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_N16: {
        dx_n16 argument = va_arg(arguments, dx_n16);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIu16, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_N32: {
        dx_n32 argument = va_arg(arguments, dx_n32);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIu32, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_N64: {
        dx_n64 argument = va_arg(arguments, dx_n64);
        char temporary[44];
        int result = snprintf(temporary, 44, "%"PRIu64, argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_F32: {
        dx_f32 argument = va_arg(arguments, dx_f32);
        char temporary[44];
        int result = snprintf(temporary, 44, "%.14g", argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_F64: {
        dx_f64 argument = va_arg(arguments, dx_f64);
        char temporary[44];
        int result = snprintf(temporary, 44, "%.14g", argument);
        if (result < 0 || result >= 44) {
          return 1;
        }
        if (dx_byte_array_append(buffer, temporary, strlen(temporary))) {
          return 1;
        }
      } break;
      case PRINT_DOLLAR: {
        static char const bytes[] = "$";
        if (dx_byte_array_append(buffer, bytes, sizeof(bytes) - 1)) {
          return 1;
        }
      } break;
      default: {
        // Expected: Format specifier. Received: Unknown format specifier prefix.
        dx_set_error(DX_INVALID_ARGUMENT);
        return 1;
      } break;
      };
      start = current;
    } else {
      current++;
    }
  }
  if (start != current) {
    if (dx_byte_array_append(buffer, start, current - start)) {
      return 1;
    }
  }
  return 0;
}
