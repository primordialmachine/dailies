#if !defined(DX_CORE_MEMORY_H_INCLUDED)
#define DX_CORE_MEMORY_H_INCLUDED

#include "dx/core/core.h"

/// @brief Assign the values of a memory block to another memory block.
/// @param p A pointer to the target memory block.
/// @param q A pointer to the source memory block.
/// @param n The number of Bytes to copy.
/// @return The zero value on success. A non-zero value on failure.
/// @warning The memory blocks denoted by <code>(p,n)</code> and <code>(q,n)</code> must not overlap.
/// @failure This function has set the error variable.
int dx_memory_copy(void* p, void const* q, dx_size n);

/// @brief Assign the values of a memory block to another memory block.
/// @param p A pointer to the target memory block.
/// @param q A pointer to the source memory block.
/// @param n The number of Bytes to copy.
/// @return The zero value on success. A non-zero value on failure.
/// @warning The memory blocks denoted by <code>(p,n)</code> and <code>(q,n)</code> may overlap.
/// @failure This function has set the error variable.
int dx_memory_move(void* p, void const* q, dx_size n);

/// @brief Assign the values of a memory block the zero values.
/// @param p A pointer to the memory block.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
int dx_memory_zero(void* p, dx_size n);

/// @brief Allocate a memory block.
/// @param n The size of the memory block. The size may be @a 0.
/// @return A pointer to the memory block on success. The null pointer on failure.
/// @failure This function has set the error variable.
void* dx_allocate(dx_size n);

/// @brief Reallocate a memory block.
/// @param p A pointer to a memory block previously allocated by calls to dx_allocate.
/// @param n The number of Bytes to reallocate the memory block to. The size may be @a 0.
/// @return A pointer to the reallocated memory block on success. The null pointer on failure.
/// @success The old memory block was deallocated (as if dx_deallocate was called with @a p as
/// the argument) and a new memory block was allocated (as if dx_allocate with argument @a n
/// was called). The first <code>min(x,y)</code> Bytes of the new memory block and the old memory
/// block are equal where <code>x</code> is the length of the old memory block and <code>y</code>
/// is the length of the new memory block.
/// @failure This function has set the error variable.
/// The old memory block was not modified.
void* dx_reallocate(void* p, dx_size n);

/// @brief Deallocate a memory block.
/// @param p A pointer to the memory block previously allocated by call to dx_allocate or the null pointer.
/// @undefined @a p does not point to a memory block previousl allocated by a call to dx_allocate and is not the null pointer.
void dx_deallocate(void* p);

#endif // DX_CORE_MEMORY_H_INCLUDED
