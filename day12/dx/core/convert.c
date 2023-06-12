#include "dx/core/convert.h"

// atod, atof
#include <stdlib.h>
// memcpy
#include <memory.h>

/// @brief Get if the current symbol is a digit.
/// @param current, end The parser.
/// @return @a true if the current symbol is a digit. @a false otherwise. 
static inline bool is_digit(char const* current, char const* end);

/// @brief Parse a floating point literal.
/// @param p A pointer to the bytes. 
/// @param n The number of Bytes.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
static int dx_parse_flit(char const* p, dx_size n);

static inline bool is_digit(char const* current, char const* end) {
  return (current != end)
      && ('0' <= *current && *current <= '9');
}

static int dx_parse_flit(char const* p, dx_size n) {
  char const* start = p;
  char const* end = p + n;
  char const* current = p;

  if (current != end) {
    if (*current == '-') {
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (is_digit(current, end)) {
    do {
      current++;
    } while (is_digit(current, end));
    if (dx_get_error()) return 1;
    if (current != end && *current == '.') {
      current++;
      while (is_digit(current, end)) {
        current++;
      }
    }
  } else if (current != end && *current == '.') {
    current++;
    if (!is_digit(current, end)) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    do {
      current++;
    } while (is_digit(current, end));
    if (dx_get_error()) return 1;
  } else {
    return 1;
  }
  if (current != end && (*current == 'e' || *current == 'E')) {
    current++;
    if (current != end && (*current == '+' || *current == '-')) {
      current++;
    }
    if (!is_digit(current, end)) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    do {
      current++;
    } while (is_digit(current, end));
    if (dx_get_error()) return 1;
  }
  return 0;
}

int dx_convert_utf8bytes_to_i8(char const* p, dx_size n, dx_i8* target) {
  if (!p || !target) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  bool negative = false;
  if (current != end) {
    if (*current == '-') {
      negative = true;
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  dx_i8 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  if (negative) {
    do {
      //
      static const dx_i8 BASE = 10;
      static const dx_i8 MIN1 = DX_I8_GREATEST + 8; // 128 / 10 = 12.8 = 12.
                                                    // 12 * 10 = 120 is no overflow.
      if (value == MIN1) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value = value * BASE;
      //
      static const dx_i8 MIN2 = DX_I8_LEAST;
      dx_i8 x = *current - '0';
      if (value < MIN2 + x) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value -= x;
      //
      current++;
    } while (is_digit(current, end));
  } else {
    do {
      //
      static const dx_i8 BASE = 10;
      static const dx_i8 MAX1 = DX_I8_GREATEST - 7; // 127 / 10 = 12.7 = 12.
                                                    // 12 * 10 = 120 is no overflow.
      if (value == MAX1) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value = value * BASE;
      //
      static const dx_i8 MAX2 = DX_I8_GREATEST;
      dx_i8 x = *current - '0';
      if (MAX2 - x < value) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value += x;
      //
      current++;
    } while (is_digit(current, end));
  }
  *target = value;
  return 0;
}

int dx_convert_utf8bytes_to_n8(char const* p, dx_size n, dx_n8* target) {
  if (!p || !target) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  if (current != end) {
    if (*current == '-') {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  dx_n8 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  do {
    //
    static const dx_n8 BASE = 10;
    static const dx_n8 MAX1 = DX_N8_GREATEST - 5; // 255 / 10 = 25.5 = 25.
                                                  // 25 * 10 = 250 is no overflow.
    if (value == MAX1) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    value = value * BASE;
    //
    static const dx_n8 MAX2 = DX_N8_GREATEST;
    dx_n8 x = *current - '0';
    if (MAX2 - x < value) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    value += x;
    //
    current++;
  } while (is_digit(current, end));
  *target = value;
  return 0;
}

int dx_convert_utf8bytes_to_i16(char const* p, dx_size n, dx_i16* target) {
  if (!p || !target) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  bool negative = false;
  if (current != end) {
    if (*current == '-') {
      negative = true;
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  dx_i16 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  if (negative) {
    do {
      //
      static const dx_i16 BASE = 10;
      static const dx_i16 MIN1 = DX_I16_LEAST - 8; // 32768 / 10 = 3276.8 = 3276.
                                                   // 3276 * 10 = 32760 is no overflow.
      if (value == MIN1) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value = value * BASE;
      //
      static const dx_i16 MIN2 = DX_I16_LEAST;
      dx_i16 x = *current - '0';
      if (value < MIN2 + x) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value -= x;
      //
      current++;
    } while (is_digit(current, end));
  } else {
    do {
      //
      static const dx_i16 BASE = 10;
      static const dx_i16 MAX1 = DX_I16_GREATEST - 7; // 32767 / 10 = 3276.7 = 3276.
                                                      // 3276 * 10 = 32760 is no overflow.
      if (value == MAX1) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value = value * BASE;
      //
      static const dx_i16 MAX2 = DX_I16_GREATEST;
      dx_i16 x = *current - '0';
      if (MAX2 - x < value) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value += x;
      //
      current++;
    } while (is_digit(current, end));
  }
  *target = value;
  return 0;
}

int dx_convert_utf8bytes_to_n16(char const* p, dx_size n, dx_n16* target) {
  if (!p || !target) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  if (current != end) {
    if (*current == '-') {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  dx_n16 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  do {
    //
    static const dx_n16 BASE = 10;
    static const dx_n16 MAX1 = DX_N16_GREATEST - 5; // 65535 / 10 = 6553.5 = 6553.
                                                    // 6553*10 = 65530 is no overflow.
    if (value == MAX1) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    value = value * BASE;
    //
    static const dx_n16 MAX2 = DX_N16_GREATEST;
    dx_n16 x = *current - '0';
    if (MAX2 - x < value) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    value += x;
    //
    current++;
  } while (is_digit(current, end));
  *target = value;
  return 0;
}

int dx_convert_utf8bytes_to_i32(char const* p, dx_size n, dx_i32* target) {
  if (!p || !target) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  bool negative = false;
  if (current != end) {
    if (*current == '-') {
      negative = true;
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  dx_i32 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  if (negative) {
    do {
      //
      static const dx_i32 BASE = 10;
      static const dx_i32 MIN1 = DX_I32_LEAST + 8; // 2147483648 / 10 = 214748364.8 = 214748364.
                                                   // 214748364 * 10 = 2147483640 is no overflow.
      if (value == MIN1) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value = value * BASE;
      //
      static const dx_i32 MIN2 = DX_I32_LEAST;
      dx_i32 x = *current - '0';
      if (value < MIN2 + x) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value -= x;
      //
      current++;
    } while (is_digit(current, end));
  } else {
    do {
      //
      static const dx_i32 BASE = 10;
      static const dx_i32 MAX1 = DX_I32_GREATEST - 7; // 2147483647 / 10 = 214748364.7 = 214748364.
                                                      // 214748364 * 10 = 2147483640 is no overflow.
      if (value == MAX1) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value = value * BASE;
      //
      static const dx_i32 MAX2 = DX_I32_GREATEST;
      dx_i32 x = *current - '0';
      if (MAX2 - x < value) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value += x;
      //
      current++;
    } while (is_digit(current, end));
  }
  *target = value;
  return 0;
}

int dx_convert_utf8bytes_to_n32(char const* p, dx_size n, dx_n32* target) {
  if (!p || !target) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  if (current != end) {
    if (*current == '-') {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  dx_n32 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  do {
    //
    static const dx_n32 BASE = 10;
    static const dx_n32 MAX1 = DX_N32_GREATEST - 5; // 4294967295 / 10 = 429496729.5 = 429496729.
                                                    // 429496729 * 10 = 4294967290 is no overflow.
    if (value == MAX1) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    value = value * BASE;
    //
    static const uint32_t MAX2 = DX_N32_GREATEST;
    dx_n32 x = *current - '0';
    if (MAX2 - x < value) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    value += x;
    //
    current++;
  } while (is_digit(current, end));
  *target = value;
  return 0;
}

int dx_convert_utf8bytes_to_i64(char const* p, dx_size n, dx_i64* target) {
  if (!p || !target) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  bool negative = false;
  if (current != end) {
    if (*current == '-') {
      negative = true;
      current++;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  dx_i64 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  if (negative) {
    do {
      //
      static const dx_i64 BASE = 10;
      static const dx_i64 MIN1 = DX_I64_LEAST + 8; // 9223372036854775808 / 10 = 922337203685477580.8 = 922337203685477580.
                                                   // 922337203685477580 * 10 = 9223372036854775800 is no overflow.
      if (value == MIN1) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value = value * BASE;
      //
      static const dx_i64 MIN2 = DX_I64_LEAST;
      dx_i64 x = *current - '0';
      if (value < MIN2 + x) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value -= x;
      //
      current++;
    } while (is_digit(current, end));
  } else {
    do {
      //
      static const dx_i64 BASE = 10;
      static const dx_i64 MAX1 = DX_I64_GREATEST - 7; // 9223372036854775807 / 10 = 922337203685477580.7 = 922337203685477580.
                                                      // 922337203685477580 * 10 = 9223372036854775800 is no overflow.
      if (value == MAX1) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value = value * BASE;
      //
      static const dx_i64 MAX2 = DX_I64_GREATEST;
      dx_i64 x = *current - '0';
      if (MAX2 - x < value) {
        dx_set_error(DX_CONVERSION_FAILED);
        return 1;
      }
      value += x;
      //
      current++;
    } while (is_digit(current, end));
  }
  *target = value;
  return 0;
}

int dx_convert_utf8bytes_to_n64(char const* p, dx_size n, dx_n64* target) {
  if (!p || !target) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* start = p;
  char const* end = p + n;
  char const* current = start;
  if (current != end) {
    if (*current == '-') {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    } else if (*current == '+') {
      current++;
    }
  }
  if (current == end) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  dx_n64 value = 0;
  // digit+
  if (!is_digit(current, end)) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  do {
    //
    static const dx_n64 BASE = 10;
    static const dx_n64 MAX1 = DX_N64_GREATEST - 5; // 18446744073709551615	/ 10 = 1844674407370955161.5 = 1844674407370955161.
                                                    // 1844674407370955161 * 10 = 18446744073709551610 is no overflow.
    if (value == MAX1) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    value = value * BASE;
    //
    static const dx_n64 MAX2 = DX_N64_GREATEST;
    dx_n64 x = *current - '0';
    if (MAX2 - x < value) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    value += x;
    //
    current++;
  } while (is_digit(current, end));
  *target = value;
  return 0;
}

int dx_convert_utf8bytes_to_sz(char const* p, dx_size n, dx_size* target) {
#if defined(_M_X64)
  return dx_convert_utf8bytes_to_n64(p, n, target);
#elif defined(_M_IX86)
  return dx_convert_utf8bytes_to_n32(p, n, target);
#else
  #error("environment not supported")
#endif
}


int dx_convert_utf8bytes_to_f32(char const* p, dx_size n, dx_f32* target) {
  if (dx_parse_flit(p, n)) {
    return 1;
  }
#define N_MAX 200
  if (n > N_MAX) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  errno = 0;
  char buffer[N_MAX + 1];
  memcpy(buffer, p, n);
  buffer[n] = '\0';
  char* end;
  double v = strtof(buffer, &end);
  if (end != &buffer[n]) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  if (errno) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
#undef N_MAX
  *target = v;
  return 0;
}

int dx_convert_utf8bytes_to_f64(char const* p, dx_size n, dx_f64* target) {
  if (dx_parse_flit(p, n)) {
    return 1;
  }
#define N_MAX 200
  if (n > N_MAX) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  errno = 0;
  char buffer[N_MAX + 1];
  memcpy(buffer, p, n);
  buffer[n] = '\0';
  char* end;
  double v = strtod(buffer, &end);
  if (end != &buffer[n]) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  if (errno) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
#undef N_MAX
  *target = v;
  return 0;
}

int dx_convert_utf8bytes_to_bool(char const* p, dx_size n, dx_bool* target) {
  if (!p || !target) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  static_assert(sizeof("true") != sizeof("false"), "environment not supported");
  switch (n) {
    case sizeof("true") - sizeof(char) : {
      *target = !memcmp(p, "true", n);
      return 0;
    } break;
    case sizeof("false") - sizeof(char) : {
      *target = !memcmp(p, "false", n);
      return 0;
    } break;
    default: {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    } break;
  };
}
