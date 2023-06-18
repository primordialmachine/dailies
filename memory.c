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
g_memory_copy
  (
    void* p,
    const void* q,
    size_t n
  )
{
  if (!p)
  {
    G_RAISE_ARGUMENT_NULL_ERROR(p);
    return false;
  }
  if (!q)
  {
    G_RAISE_ARGUMENT_NULL_ERROR(q);
    return false;
  }
  memcpy(p, q, n);
  return true;
}

bool
g_memory_zero
  (
    void* p,
    size_t n
  )
{
#if defined(_DEBUG)
  if (!p)
  {
    G_RAISE_ARGUMENT_NULL_ERROR(p);
    return false;
  }
#endif
  memset(p, 0, n);
  return true;
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
g_memory_allocate
  (
    size_t n
  )
{
  void *p = malloc(n > 0 ? n : 1);
  if (!p)
  {
    G_ERROR(G_FAILURE, "allocation of %zu Bytes failed\n", n);
    return NULL;
  }
  return p;
}

void*
g_memory_reallocate
  (
    void *p,
    size_t n
  )
{
#if defined(_DEBUG)
  if (!p)
  {
    G_RAISE_ARGUMENT_NULL_ERROR(p);
    return NULL;
  }
#endif
  void *q = realloc(p, n > 0 ? n : 1);
  if (!q)
  {
    G_ERROR(G_FAILURE, "reallocation to %zu Bytes failed\n", n);
    return NULL;
  }
  return q;
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

void
g_memory_deallocate
  (
    void* p
  )
{
  if (p != NULL)
  {
    free(p);
  }
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
