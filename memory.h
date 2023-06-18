/* Copyright (c) 2020 Michael Heilmann. All rights reserved. */
#if !defined(G_MEMORY_H_INCLUDED)
#define G_MEMORY_H_INCLUDED

#include "last-men-standing/compiler.h"

/// @return @a true on success, @a false on failure. 
bool
g_memory_zero_array
  (
    void* p,
    size_t n,
    size_t m
  );


/// @brief Allocate an array.
/// @param n The element size.
/// @param m The number of elements.
/// @return A pointer to the array on success, a null pointer on failure.
void*
g_memory_allocate_array
  (
    size_t n,
    size_t m
  );

/// @brief Reallocate a memory block previously allocated by calls to g_memory_allocate or g_memory_allocate_array.
/// @param p A pointer to a memory block.
/// @param n, m The number of Bytes to reallocate the memory block to. 
/// @return A pointer to the reallocated memory block.
void*
g_memory_reallocate_array
  (
    void* p,
    size_t n,
    size_t m
  );

/// @brief Compare prefixes two memory blocks of the same size.
/// @param a, b Pointers to the memory blocks.
/// @param n The size of the prefix.
/// @return
/// - @a 0 if each Byte value in the prefix of the 1st memory block is equal to its corresponding Byte value in the prefix of the 2nd memory block.
/// - A negative (positive) value is returned if the first non-matching Byte value is smaller (greater) than its corresponding Byte value in the 2nd memory block.
int
g_memory_compare
  (
    const void* a,
    const void* b,
    size_t n
  );

#endif // G_MEMORY_H_INCLUDED
