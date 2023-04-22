// Copyright (c) 2018-2023 Michael Heilmann
#include "dx/core/pointer_array.h"

// malloc, realloc, free
#include <malloc.h>

// memmove
#include <memory.h>

#include "dx/core/safe_add_nx.h"
#include "dx/core/safe_mul_nx.h"
#include "dx/core/next_power_of_two.h"

/// @brief The greatest capacity, in elements, of a pointer array.
static dx_size const GREATEST_CAPACITY = DX_SIZE_GREATEST / sizeof(void *);

/// @brief The least capacity, in elements, of a pointer array.
static dx_size const LEAST_CAPACITY = 0;

static dx_size dx_get_best_array_size(dx_size current, dx_size additional, size_t least, size_t greatest, bool saturate);

static dx_size dx_get_best_array_size(dx_size current, dx_size additional, size_t least, size_t greatest, bool saturate) {
  dx_error old_error = dx_get_error();
  if (least > greatest) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (DX_SIZE_GREATEST - current < additional) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  size_t new = current;
  if (new < least) {
    new = least;
  }
  new = current + additional;
  size_t new1 = dx_next_power_of_two_sz(new);
  if (dx_get_error()) {
    dx_set_error(old_error);
  }
  new = new1;
  if (new > greatest) {
    if (!saturate) {
      dx_set_error(DX_NOT_FOUND);
      return 1;
    }
    new = greatest;
    if (new < current + additional) {
      dx_set_error(DX_NOT_FOUND);
      return 1;   
    }
  }
  return new;
}

int
dx_pointer_array_initialize
  (
    dx_pointer_array *self,
    dx_size initial_capacity,
    dx_added_callback* added_callback,
    dx_removed_callback* removed_callback
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  size_t overflow;
  dx_size initial_capacity_bytes = dx_mul_sz(initial_capacity, sizeof(dx_pointer_array_element), &overflow);
  if (overflow) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;  
  }
  void **elements = malloc(initial_capacity_bytes > 0 ? initial_capacity_bytes : 1);
  if (!elements) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
 self->size = 0;
 self->elements = elements;
 self->capacity = initial_capacity;
 self->added_callback = added_callback;
 self->removed_callback = removed_callback;
 return 0;
}

void
dx_pointer_array_uninitialize
  (
    dx_pointer_array* self
  )
{
  dx_pointer_array_clear(self);
  free(self->elements);
  self->elements = NULL;
  self->capacity = 0;
}

int
dx_pointer_array_increase_capacity
  (
    dx_pointer_array* self,
    dx_size additional_capacity
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (!additional_capacity) {
    return 0;
  }
  dx_set_error(DX_NO_ERROR);
  size_t new_capacity = dx_get_best_array_size(self->capacity, additional_capacity, LEAST_CAPACITY, GREATEST_CAPACITY, true);
  if (dx_get_error()) {
    return 1;
  }
  size_t overflow;
  size_t new_capacity_bytes = dx_mul_sz(new_capacity, sizeof(dx_pointer_array_element), &overflow);
  if (overflow) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;  
  }
  dx_pointer_array_element *new_elements = realloc(self->elements, new_capacity_bytes > 0 ? 1 : 0);
  if (!new_elements) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;    
  }
  self->capacity = new_capacity;
  self->elements = new_elements;
  return 0;
}

int
dx_pointer_array_ensure_free_capacity
  (
    dx_pointer_array* self,
    dx_size required_free_capacity
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;   
  }
  dx_size available_free_capacity = self->capacity - self->size;
  if (available_free_capacity > required_free_capacity) {
    return 0;
  }
  size_t additional_capacity = required_free_capacity - available_free_capacity;
  return dx_pointer_array_increase_capacity(self, additional_capacity);
}

int
dx_pointer_array_append
  (
    dx_pointer_array* self,
    dx_pointer_array_element pointer
  )
{
  if (!self || !pointer) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_pointer_array_insert(self, pointer, self->size);
}

int
dx_pointer_array_prepend
  (
    dx_pointer_array* self,
    dx_pointer_array_element pointer
  )
{
  if (!self || !pointer) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_pointer_array_insert(self, pointer, 0);
}

int
dx_pointer_array_insert
  (
    dx_pointer_array* self,
    dx_pointer_array_element pointer,
    dx_size index
  )
{
  if (!self || !pointer) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (index > self->size) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;  
  }
  if (dx_pointer_array_ensure_free_capacity(self, 1)) {
    return 1;
  }
  if (self->added_callback) {
    self->added_callback(&pointer);
  }
  if (index != self->size) {
    memmove(self->elements + index + 1,
            self->elements + index + 0,
            (self->size - index) * sizeof(dx_pointer_array_element));
  }
  self->elements[index] = pointer;
  self->size++;
  return 0;
}

dx_pointer_array_element dx_pointer_array_get_at(dx_pointer_array* self, size_t index) {
  if (!self || index >= self->size) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  return self->elements[index];
}

dx_size dx_pointer_array_get_size(dx_pointer_array const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return self->size;
}

dx_size dx_pointer_array_get_capacity(dx_pointer_array const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return self->capacity;
}

dx_size dx_pointer_array_get_free_capacity(dx_pointer_array const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return self->capacity - self->size;
}

int dx_pointer_array_clear(dx_pointer_array* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->removed_callback) {
    dx_removed_callback *removed_callback = self->removed_callback;
    while (self->size > 0) {
      void *element = self->elements[--self->size];
      removed_callback(&element);
    }
  } else {
    self->size = 0;
  }
  return 0;
}
