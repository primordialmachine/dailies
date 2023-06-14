#include "dx/core/object_array.h"

static void added_callback(dx_object **p) {
  if (*p) DX_REFERENCE(*p);
}

static void removed_callback(dx_object** p) {
  if (*p) DX_UNREFERENCE(*p);
}

int dx_object_array_initialize(dx_object_array* self, dx_size initial_capacity) {
  if (dx_pointer_array_initialize(&self->backend, initial_capacity, (void(*)(void*))&added_callback, (void(*)(void*))&removed_callback)) {
    return 1;
  }
  return 0;
}

void dx_object_array_uninitialize(dx_object_array* self) {
  dx_pointer_array_uninitialize(&self->backend);
}

int
dx_object_array_increase_capacity
  (
    dx_object_array* self,
    dx_size additional_capacity
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_pointer_array_increase_capacity(&self->backend, additional_capacity);
}

int
dx_object_array_ensure_free_capacity
  (
    dx_object_array* self,
    dx_size required_free_capacity
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_pointer_array_ensure_free_capacity(&self->backend, required_free_capacity);
}

int
dx_object_array_append
  (
    dx_object_array* self,
    dx_object *pointer
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_pointer_array_append(&self->backend, pointer);
}

int
dx_object_array_prepend
  (
    dx_object_array* self,
    dx_object *pointer
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_pointer_array_prepend(&self->backend, pointer);
}

int
dx_object_array_insert
  (
    dx_object_array* self,
    dx_object *pointer,
    dx_size index
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_pointer_array_insert(&self->backend, pointer, index);
}

dx_object*
dx_object_array_get_at
  (
    dx_object_array* self,
    dx_size index
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  return dx_pointer_array_get_at(&self->backend, index);
}

dx_size
dx_object_array_get_size
  (
    dx_object_array const* self
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return dx_pointer_array_get_size(&self->backend);
}

dx_size
dx_object_array_get_capacity
  (
    dx_object_array const* self
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return dx_pointer_array_get_capacity(&self->backend);
}

dx_size
dx_object_array_get_free_capacity
  (
    dx_object_array const* self
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return dx_pointer_array_get_free_capacity(&self->backend);
}

int
dx_object_array_clear
  (
    dx_object_array* self
  )
{
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_pointer_array_clear(&self->backend);
}
