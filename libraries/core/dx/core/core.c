#include "dx/core/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// fwrite
#include <stdio.h>

// isnan, fabsf, fabs
#include <math.h>

// InterlockedIncrement64, InterlockedDecrement64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "dx/core/safe_mul_nx.h"
#include "dx/core/safe_add_nx.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_THREAD_LOCAL() dx_error g_error = DX_NO_ERROR;

dx_error dx_get_error() {
  return g_error;
}

void dx_set_error(dx_error error) {
  g_error = error;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_reference_counter dx_reference_counter_increment(dx_reference_counter* reference_counter) {
#if 1 == DX_ATOMIC_REFERENCE_COUNTING_ENABLED
  #if defined(_WIN64)
    return InterlockedIncrement64(reference_counter);
  #else
    return InterlockedIncrement(reference_counter);
  #endif
#else
  return ++(*reference_counter);
#endif
}

dx_reference_counter dx_reference_counter_decrement(dx_reference_counter* reference_counter) {
#if 1 == DX_ATOMIC_REFERENCE_COUNTING_ENABLED
  #if defined(_WIN64)
    return InterlockedDecrement64(reference_counter);
  #else
    return InterlockedDecrement(reference_counter);
  #endif
#else
  return --(*reference_counter);
#endif
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


void dx_log_i64(dx_i64 value) {
  fprintf(stdout, "%"PRIi64, value);
}

void dx_log_i32(dx_i32 value) {
  fprintf(stdout, "%"PRIi32, value);
}

void dx_log_i16(dx_i16 value) {
  fprintf(stdout, "%"PRIi16, value);
}

void dx_log_i8(dx_i8 value) {
  fprintf(stdout, "%"PRIi8, value);
}


void dx_log_n64(dx_n64 value) {
  fprintf(stdout, "%"PRIu64, value);
}

void dx_log_n32(dx_n32 value) {
  fprintf(stdout, "%"PRIu32, value);
}

void dx_log_n16(dx_n16 value) {
  fprintf(stdout, "%"PRIu16, value);
}

void dx_log_n8(dx_n8 value) {
  fprintf(stdout, "%"PRIu8, value);
}


void dx_log_p(void const* p) {
  fprintf(stdout, "%p", p);
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

dx_size dx_combine_hash(dx_size x, dx_size y) {
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
