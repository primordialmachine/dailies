#include "dx/core/byte_array.h"

// malloc, free
#include <malloc.h>
// memcpy
#include <memory.h>

int dx_byte_array_construct(dx_byte_array* byte_array) {
  if (!byte_array) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  byte_array->elements = malloc(1);
  if (!byte_array->elements) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  byte_array->size = 0;
  byte_array->capacity = 0;
  return 0;
}

void dx_byte_array_destruct(dx_byte_array* byte_array) {
  free(byte_array->elements);
  byte_array->elements = NULL;
}

int dx_byte_array_steal(dx_byte_array* byte_array, char **bytes, dx_size *number_of_bytes) {
  if (byte_array->size < byte_array->capacity) {
    char *new_elements = realloc(byte_array->elements, byte_array->size);
    if (!new_elements) {
      dx_set_error(DX_ALLOCATION_FAILED);
      return 1;
    }
    byte_array->elements = new_elements;
    byte_array->capacity = byte_array->size;
  }
  char *new_elements = malloc(1);
  if (!new_elements) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  *bytes = byte_array->elements;
  *number_of_bytes = byte_array->size;
  byte_array->size = 0;
  byte_array->capacity = 0;
  byte_array->elements = new_elements;
  return 0;
}

int dx_byte_array_increase_capacity(dx_byte_array* self, dx_size additional) {
  dx_size old = self->capacity;
  //dx_size additional = required - available;
  if (SIZE_MAX - old < additional) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  dx_size new = old + additional;
  // optimize the allocations
  dx_size best_new;
  if (SIZE_MAX / 2 >= old) {
    // To understand the above test, simply consider uint8_t instead of dx_size as the types.
    // Then the expression "UINT8_MAX / 2"  is evaluated to floor(255 / 2) = floor(255/2) = floor(127.5) = 127.
    // If the old capacity is smaller than or equal to 127, it can be safely double as 127 * 2 = 254.
    best_new = old > 0 ? 2 * old : 8;
  } else {
    best_new = SIZE_MAX;
  }
  if (new < best_new) {
    new = best_new;
  }
  char* new_elements = realloc(self->elements, new);
  if (!new_elements) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  self->elements = new_elements;
  self->capacity = new;
  return 0; 
}

int dx_byte_array_ensure_free_capacity(dx_byte_array* self, dx_size required) {
  dx_size available = self->capacity - self->size;
  if (available >= required) {
    return 0;
  }
  dx_size additional = required - available;
  return dx_byte_array_increase_capacity(self, additional);
}

int dx_byte_array_prepend(dx_byte_array* self, char const* bytes, dx_size number_of_bytes) {
  return dx_byte_array_insert(self, 0, bytes, number_of_bytes);
}

int dx_byte_array_append(dx_byte_array* self, char const* bytes, dx_size number_of_bytes) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_byte_array_insert(self, self->size, bytes, number_of_bytes);
}
 
int dx_byte_array_insert(dx_byte_array* self, dx_size index, char const* bytes, dx_size number_of_bytes) {
  if (!self || !bytes) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (dx_byte_array_ensure_free_capacity(self, number_of_bytes)) {
    return 1;
  }
  if (index < self->size) {
    memmove(self->elements + index + number_of_bytes, self->elements + index, number_of_bytes);
  }
  memcpy(self->elements + index, bytes, number_of_bytes);
  self->size += number_of_bytes;
  return 0;
}

dx_size dx_byte_array_get_size(dx_byte_array const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return self->size;
}

void* dx_byte_array_get_bytes(dx_byte_array* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  return self->elements;
}

dx_size dx_byte_array_get_capacity(dx_byte_array const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return self->capacity;
}

dx_size dx_byte_array_get_free_capacity(dx_byte_array const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return self->capacity - self->size;
}

int dx_byte_array_clear(dx_byte_array* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->size = 0;
  return 0;
}
