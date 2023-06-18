#include "dx/core/memory.h"

// malloc, realloc, free
#include <malloc.h>

// memmove, memcpy, memset
#include <string.h>

int dx_memory_copy(void* p, void const* q, dx_size n) {
  if (!p || !q) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  memcpy(p, q, n);
  return 0;
}

int dx_memory_move(void* p, void const* q, dx_size n) {
  if (!p || !q) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  memmove(p, q, n);
  return 0;
}

int dx_memory_zero(void* p, dx_size n) {
  if (!p) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  memset(p, 0, n);
  return 0;
}

void* dx_memory_allocate(dx_size n) {
  void* p = malloc(n > 0 ? n : 1);
  if (!p) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return NULL;
  }
  return p;
}

void* dx_memory_reallocate(void* p, dx_size n) {
  if (!p) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  void *q = realloc(p, n > 0 ? n : 1);
  if (!q) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return  NULL;
  }
  return q;
}

void dx_memory_deallocate(void* p) {
  if (p) {
    free(p);
  }
}

int dx_memory_compare(void const* p, void const* q, dx_size n) {
  if (!p || !q) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return !memcmp(p, q, n);
}
