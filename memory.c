#include "last-men-standing/memory.h"

#include "last-men-standing/result.h"
#include <malloc.h>
#include <limits.h>
#include <string.h>

/// @brief Determine if the product of two factors would overflow.
/// @param n, m The factors.
/// @return @a true if the product <code>n*m</code> would overflow. 
static inline bool is_overflow(size_t n, size_t m)
{
  return n != 0 && SIZE_MAX / n < m;
}

bool
g_memory_zero_array
  (
    void* p,
    size_t n,
    size_t m
  )
{
#if defined(_DEBUG)
  if (!p)
  {
    G_RAISE_ARGUMENT_NULL_ERROR(p);
    return false;
  }
  if (is_overflow(n, m))
  {
    G_ARGUMENT_OUT_OF_RANGE_ERROR(G_FAILURE, n * m);
    return false;
  }
#endif
  size_t k = n * m;
  memset(p, 0, k);
  return true;
}

void*
g_memory_allocate_array
  (
    size_t n,
    size_t m
  )
{
#if defined(_DEBUG)
  if (is_overflow(n, m))
  {
    G_ERROR(G_FAILURE, "allocation of %zu Bytes failed\n", n);
    return NULL;
  }
#endif
  size_t k = n * m;
  void *p = malloc(k);
  if (!p)
  {
      G_ERROR(G_FAILURE, "allocation of %zu*%zu Bytes failed\n", n, m);
      return NULL;
  }
  return p;
}

void*
g_memory_reallocate_array
  (
    void* p,
    size_t n,
    size_t m
  )
{
#if defined(_DEBUG)
  if (!p)
  {
    G_RAISE_ARGUMENT_NULL_ERROR(p);
    return NULL;
  }
  if (is_overflow(n, m))
  {
    G_ERROR(G_FAILURE, "reallocation of %zu * %zu Bytes failed\n", n, m);
    return NULL;
  }
#endif
  size_t k = n * m;
  void* q = realloc(p, k > 0 ? k : 1);
  if (!q)
  {
    G_ERROR(G_FAILURE, "reallocation to %zu * %zu Bytes failed\n", n, m);
    return NULL;
  }
  return q;
}

int
g_memory_compare
  (
    const void* a,
    const void* b,
    size_t n
  )
{
  return memcmp(a, b, n);
}
