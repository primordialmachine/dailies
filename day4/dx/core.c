#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// fwrite
#include <stdio.h>

// malloc, free
#include <malloc.h>

// isnan, fabsf, fabs
#include <math.h>

// InterlockedIncrement64, InterlockedDecrement64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_error g_error = DX_NO_ERROR;

dx_error dx_get_error() {
  return g_error;
}

void dx_set_error(dx_error error) {
  g_error = error;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_reference_counter dx_reference_counter_increment(dx_reference_counter* reference_counter) {
  return InterlockedIncrement64(reference_counter);
}

dx_reference_counter dx_reference_counter_decrement(dx_reference_counter* reference_counter) {
  return InterlockedDecrement64(reference_counter);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_object* dx_object_alloc(size_t size) {
  if (size < sizeof(dx_object)) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  dx_object* object = malloc(size);
  if (!object) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return NULL;
  }
  object->reference_count = 1;
  object->destruct = NULL;
  return object;
}

void dx_object_reference(dx_object *object) {
  dx_reference_counter_increment(&object->reference_count);
}

void dx_object_unreference(dx_object* object) {
  if (!dx_reference_counter_decrement(&object->reference_count)) {
    if (object->destruct) {
      object->destruct(object);
    }
    free(object);
    object = NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void dx_log(char const *p, size_t n) {
  if (!p || !n) {
    return;
  } else {
   if (n != fwrite(p, 1, n, stdout)) {
    return;
   }
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

uint32_t dx_msg_get_flags(dx_msg const* msg) {
  return msg->flags;
}

int dx_msg_construct(dx_msg* msg) {
  dx_log("enter: dx_msg_construct\n", sizeof("enter: dx_msg_construct\n"));
  msg->flags = DX_MSG_TYPE_UNDETERMINED;
  DX_OBJECT(msg)->destruct = (void (*)(dx_object*)) & dx_msg_destruct;
  dx_log("leave: dx_msg_construct\n", sizeof("leave: dx_msg_construct\n"));
  return 0;
}

void dx_msg_destruct(dx_msg* msg)
{/*Intentionally empty.*/}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct dx_emit_msg {
  dx_msg _parent;
  char* p;
  size_t n;
};

int dx_emit_msg_construct(dx_emit_msg* emit_msg, char const *p, size_t n) {
  if (dx_msg_construct(DX_MSG(emit_msg))) {
    return 1;
  }
  emit_msg->p = malloc(n > 0 ? n : 1);
  if (!emit_msg->p) {
    dx_set_error(DX_ALLOCATION_FAILED);
    dx_msg_destruct(DX_MSG(emit_msg));
    return 1;
  }
  memcpy(emit_msg->p, p, n);
  emit_msg->n = n;
  DX_MSG(emit_msg)->flags = DX_MSG_TYPE_EMIT;
  DX_OBJECT(emit_msg)->destruct = (void (*)(dx_object*)) & dx_emit_msg_destruct;
  return 0;
}

void dx_emit_msg_destruct(dx_emit_msg* emit_msg) {
  free(emit_msg->p);
  emit_msg->p = NULL;
  dx_msg_destruct(DX_MSG(emit_msg));
}

dx_emit_msg* dx_emit_msg_create(char const *p, size_t n) {
  dx_emit_msg* emit_msg = DX_EMIT_MSG(dx_object_alloc(sizeof(dx_emit_msg)));
  if (!emit_msg) {
    return NULL;
  }
  if (dx_emit_msg_construct(emit_msg, p, n)) {
    DX_UNREFERENCE(emit_msg);
    emit_msg = NULL;
    return NULL;
  }
  return emit_msg;
}

int dx_emit_msg_get(dx_emit_msg* emit_msg, char const** p, size_t* n) {
  *p = emit_msg->p;
  *n = emit_msg->n;
  return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool dx_almost_equal_f32(float a, float b, int method, float epsilon) {
  if (isnan(a) && isnan(b)) {
    return true;
  }
  if (isnan(epsilon)) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  switch (method) {
  case 1:
    return a == b;
  case 2:
    return fabsf(a - b) <= epsilon;
  case 3:
    return fabsf(a - b) <= (epsilon * max(fabsf(a), fabsf(b)));
  default:
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
}

bool dx_almost_equal_f64(double a, double b, int method, double epsilon) {
  if (isnan(a) && isnan(b)) {
    return true;
  }
  if (isnan(epsilon)) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  switch (method) {
  case 1:
    return a == b;
  case 2:
    return fabs(a - b) <= epsilon;
  case 3:
    return fabs(a - b) <= (epsilon * max(fabs(a), fabs(b)));
  default:
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

size_t dx_os_get_page_size() {
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  if (system_info.dwPageSize > SIZE_MAX) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 0;
  }
  return (size_t)system_info.dwPageSize;
}

size_t dx_os_get_number_of_cores() {
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  if (system_info.dwNumberOfProcessors > SIZE_MAX) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 0;
  }
  return (size_t)system_info.dwNumberOfProcessors;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_size dx_hash_pointer(void const* p) {
  return (dx_size)(uintptr_t)p;
}

dx_size dx_hash_bool(dx_bool x) {
  return x ? 1231 : 1237; // These are arbitrary prime number as e.g. used in Boolean.getHashCode in Java. 
}

dx_size dx_hash_bytes(void const* p, dx_size n) {
  dx_size h = n;
  dx_size step = (n >> 5) + 1;
  for (; n >= step; n -= step) {
    h ^= (h << 5) + (h >> 2) + ((unsigned char)(((char const*)p)[n - 1]));
  }
  return h;
}

dx_size dx_hash_f32(dx_f32 x) {
  switch (fpclassify(x)) {
    case FP_INFINITE: { 
      return x > 0.0f ? -1 : -2;
    }
    case FP_NAN: {
      return (size_t)-3;
    }
    case FP_ZERO: { 
      return 0;
    }
    case FP_NORMAL:
    case FP_SUBNORMAL:
    default: {
      // The default case handles the case when fpclassify may return an implementation defined type gracefully.
      if (x == 0.0f) x = +0.0f; // +0.0f == -0.0f. Hence, map both to +0.0f.
      return dx_hash_bytes((void const *)&x, sizeof(dx_f32));
    }
  };
}

dx_size dx_hash_f64(dx_f64 x) {
  switch (fpclassify(x)) {
    case FP_INFINITE: { 
      return x > 0.0 ? -1 : -2;
    }
    case FP_NAN: {
      return (size_t)-3;
    }
    case FP_ZERO: { 
      return 0;
    }
    case FP_NORMAL:
    case FP_SUBNORMAL:
    default: {
      // The default case handles the case when fpclassify may return an implementation defined type gracefully.
      if (x == 0.0) x = +0.0; // +0.0 == -0.0. Hence, map both to +0.0.
      return dx_hash_bytes((void const *)&x, sizeof(dx_f64));
    }
  };
}

dx_size dx_hash_i8(dx_i8 x)
{ return dx_hash_bytes((void const*)&x, sizeof(dx_i8)); }

dx_size dx_hash_i16(dx_i16 x)
{ return dx_hash_bytes((void const*)&x, sizeof(dx_i16)); }

dx_size dx_hash_i32(dx_i32 x)
{ return dx_hash_bytes((void const*)&x, sizeof(dx_i32)); }

dx_size dx_hash_i64(dx_i64 x)
{ return dx_hash_bytes((void const*)&x, sizeof(dx_i64)); }

dx_size dx_hash_n8(dx_n8 x)
{ return dx_hash_bytes((void const*)&x, sizeof(dx_n8)); }

dx_size dx_hash_n16(dx_n16 x)
{ return dx_hash_bytes((void const*)&x, sizeof(dx_n16)); }

dx_size dx_hash_n32(dx_n32 x)
{ return dx_hash_bytes((void const*)&x, sizeof(dx_n32)); }

dx_size dx_hash_n64(dx_n64 x)
{ return dx_hash_bytes((void const*)&x, sizeof(dx_n64)); }

dx_size dx_hash_sz(dx_size x)
{ return dx_hash_bytes((void const*)&x, sizeof(dx_size)); }

dx_size_dx_combine_hash(dx_size x, dx_size y) {
  // This is just another black-magic formula used by boost and glib.
  return  x + 0x9e3779b9 + (y << 6) + (y >> 2);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_bool dx_fp32_is_subnormal(dx_f32 x) {
  return FP_SUBNORMAL == fpclassify(x);
}

dx_bool dx_fp64_is_subnormal(dx_f64 x) {
  return FP_SUBNORMAL == fpclassify(x);
}

dx_bool dx_fp32_is_nan(dx_f32 x) {
  return FP_NAN == fpclassify(x);
}

dx_bool dx_fp64_is_nan(dx_f64 x) {
  return FP_NAN == fpclassify(x);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
