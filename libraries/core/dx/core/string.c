#include "dx/core/string.h"

#include "dx/core/_string_format.h"

// memcmp, memcpy
#include <string.h>

#include "dx/core/byte_array.h"
#include "dx/core/safe_add_nx.h"
#include "dx/core/safe_mul_nx.h"

dx_string* dx_string_printfv(dx_string* format, va_list arguments) {
  dx_byte_array buffer;
  if (dx_byte_array_initialize(&buffer)) {
    return NULL;
  }
  if (dx__format_v(&buffer, format->bytes, format->bytes + format->number_of_bytes, arguments)) {
    dx_byte_array_uninitialize(&buffer);
    return NULL;
  }
  dx_string *string = dx_string_create(buffer.elements, buffer.size);
  dx_byte_array_uninitialize(&buffer);
  return string;
}

dx_string* dx_string_printf(dx_string* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  dx_string* string = dx_string_printfv(format, arguments);
  va_end(arguments);
  return string;
}

int dx_string_construct(dx_string* self, char const* bytes, dx_size number_of_bytes) {
  if (!self || !bytes) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  memcpy(self->bytes, bytes, number_of_bytes);
  self->number_of_bytes = number_of_bytes;
  return 0;
}

dx_string* dx_string_create(char const *bytes, dx_size number_of_bytes) {
  if (SIZE_MAX - sizeof(dx_string) < number_of_bytes) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  dx_string *self = DX_STRING(dx_object_alloc(sizeof(dx_string) + number_of_bytes));
  if (!self) {
    return NULL;
  }
  if (dx_string_construct(self, bytes, number_of_bytes)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

void const* dx_string_get_bytes(dx_string const* self) {
  return self->bytes;
}

dx_size dx_string_get_number_of_bytes(dx_string const* self) {
  return self->number_of_bytes;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// Classify the first Byte of an UTF8 sequence to determine the length of the sequence.
/// @param x
/// The Byte.
/// @return
/// The length of the sequence.
/// The zero value on failure.
/// The function fails if there is an encoding error.
/// @failure This function has set the error variable to #DX_DECODING_FAILED.
static dx_size classify(uint8_t x) {
  if ((x & 0x80) == 0x00) {
    // To determine if the first Byte is in the range 0xxx xxxx,
    // mask the Byte with 1000 0000 / 0x80. If the result is 0,
    // then the first Byte is in the range 0xxx xxxx.
    return 1;
  }
  else if ((x & 0xE0) == 0xC0) {
    // To determine if the first Byte is in the range 110x xxxx,
    // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
    // then the first Byte is in the range 110x xxxx.
    return 2;
  }
  else if ((x & 0xF0) == 0xE0) {
    // To determine if the first Byte is in the range 1110 xxxx,
    // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
    // then the first Byte is in the range 1110 xxxx.
    return 3;
  }
  else if ((x & 0xF8) == 0xF0) {
    // To determine if the first Byte is in the range 1111 0xxx,
    // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
    // then the first Byte is in th range 1111 0xxx.
    return 4;
  }
  else {
    dx_set_error(DX_DECODING_FAILED);
    return 0;
  }
}

typedef struct y_utf8_it {
  uint8_t const* current;
  uint8_t const* end;
  bool error;
} y_utf8_it;

int y_utf8_decode(y_utf8_it* it, dx_size* n, uint32_t* p) {
  if (it->error || it->current == it->end) {
    return 1;
  }
  dx_size l = classify(*it->current);
  if (!l) {
    it->error = true;
    return 1;
  }
  if (it->current + l > it->end) {
    it->error = true;
    return 1;
  }
  switch (l) {
  case 1: {
    *p = it->current[0];
  } break;
  case 2: {
    *p = (it->current[0] << 8)
      | (it->current[1] << 0);
  } break;
  case 3: {
    *p = (it->current[0] << 16)
      | (it->current[1] << 8)
      | (it->current[2] << 0);
  } break;
  case 4: {
    *p = (it->current[0] << 24)
      | (it->current[1] << 16)
      | (it->current[2] << 8)
      | (it->current[3] << 0);
  } break;
  };
  *n = l;
  it->current += l;
  return 0;
}

bool dx_string_contains_symbol(dx_string const* self, uint32_t symbol) {
  y_utf8_it it = { .current = (uint8_t*)self->bytes,
                   .end = (uint8_t*)self->bytes + self->number_of_bytes,
                   .error = false };
  uint32_t p;
  dx_size n;
  while (!y_utf8_decode(&it, &n, &p)) {
    if (p == symbol) {
      return true;
    }
  }
  return false;
}

bool dx_string_is_equal_to(dx_string const* self, dx_string const* other) {
  if (!self || !other) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  if (self->number_of_bytes != other->number_of_bytes) {
    return false;
  }
  return !memcmp(self->bytes, other->bytes, self->number_of_bytes);
}

dx_size dx_string_get_hash_value(dx_string const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return dx_hash_bytes(self->bytes, self->number_of_bytes);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_str_to_i64(char const* p, dx_size l, int64_t* v) {
  if (!p || !v) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* q = p + l;
  bool negative = false;
  if (p != q && *p == '-') {
    negative = true;
    p++;
    l -= 1;
  }
  uint64_t u;
  if (dx_str_to_u64(p, l, &u)) {
    return 1;
  }
  // neither +u nor -u can be represented by an int64_t value.
  if (u > INT64_MAX) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  // u is within the range of [0,INT64_MAX].
  int64_t w = negative ? -(int64_t)u : +(int64_t)u;
  *v = w;
  return 0;
}

int dx_str_to_u64(char const* p, dx_size l, uint64_t* v) {
  if (!p || !v) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  char const* q = p + l;
  bool negative = false;
  if (p != q && *p == '+') {
    p++;
  } else if (p != q && *p == '-') {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  uint64_t u = 0;
  // there must be at least one digit
  if (!(p != q && '0' <= *p && *p <= '9')) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  do {
    uint64_t overflow;
    // safe multiply
    u = dx_mul_u64(u, 10, &overflow);
    if (overflow) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    uint64_t w = (*p) - '0';
    // safe add
    u = dx_add_u64(u, w, &overflow);
    if (overflow) {
      dx_set_error(DX_CONVERSION_FAILED);
      return 1;
    }
    p++;
  } while (p != q && '0' <= *p && *p <= '9');
  if (p != q) {
    dx_set_error(DX_CONVERSION_FAILED);
    return 1;
  }
  *v = u;
  return 0;
}
