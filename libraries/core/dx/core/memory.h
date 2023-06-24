#if !defined(DX_CORE_MEMORY_H_INCLUDED)
#define DX_CORE_MEMORY_H_INCLUDED

#include "dx/core/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Assign the values of an object to another object.
/// @param p A pointer to the target object.
/// @param q A pointer to the source object.
/// @param n The number of Bytes to copy.
/// @return The zero value on success. A non-zero value on failure.
/// @warning The objects denoted by <code>(p,n)</code> and <code>(q,n)</code> must not overlap.
/// @failure This function has set the error variable.
int dx_memory_copy(void* p, void const* q, dx_size n);

/// @brief Assign the values of an object to another object.
/// @param p A pointer to the target object.
/// @param q A pointer to the source object.
/// @param n The number of Bytes to copy.
/// @return The zero value on success. A non-zero value on failure.
/// @warning The object denoted by <code>(p,n)</code> and <code>(q,n)</code> may overlap.
/// @failure This function has set the error variable.
int dx_memory_move(void* p, void const* q, dx_size n);

/// @brief Assign the values of an object block the zero values.
/// @param p A pointer to the memory block.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
int dx_memory_zero(void* p, dx_size n);

/// @brief Allocate an object.
/// @param n The size of the object. The size may be @a 0.
/// @return A pointer to the object on success. The null pointer on failure.
/// @failure This function has set the error variable.
void* dx_memory_allocate(dx_size n);

/// @brief Reallocate an object.
/// @param p A pointer to an object previously allocated by a call to dx_allocate.
/// @param n The number of Bytes to reallocate the object to. The size may be @a 0.
/// @return A pointer to the reallocated object on success. The null pointer on failure.
/// @success The old object was deallocated (as if dx_deallocate was called with @a p as
/// the argument) and a new object was allocated (as if dx_allocate with argument @a n
/// was called). The first <code>min(x,y)</code> Bytes of the new object and the old object
/// are equal where <code>x</code> is the length of the old object and <code>y</code>
/// is the length of the new object.
/// @failure This function has set the error variable.
/// The old memory block was not modified.
void* dx_memory_reallocate(void* p, dx_size n);

/// @brief Deallocate an object.
/// @param p A pointer to the object previously allocated by call to dx_allocate or the null pointer.
/// @undefined @a p does not point to a object previousl allocated by a call to dx_allocate and is not the null pointer.
void dx_memory_deallocate(void* p);

/// @brief Compare the values of objects.
/// @details This functin compares the Byte values of the objects lexicographically.
/// @param p A pointer to the first object.
/// @param q A pointer to the second object.
/// @param n The number of Bytes to compare.
/// @return
/// The zero value if the memory blocks are lexicographically equal.
/// A negative value if the first memory block is lexicographically smaller.
/// A positive value if the first memory block is lexicographically greater.
int dx_memory_compare(void const* p, void const* q, dx_size n);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Statistics of an allocator.
typedef struct dx_allocator_statistics dx_allocator_statistics;

struct dx_allocator_statistics {
  dx_size smallest_block;       ///< The size, in Bytes, of the greatest block allocated.
                                ///< Can be @a 0 even if Blocks are allocated as Blocks can have zero size.
                                ///< If no blocks are currently allocated, then the value is @a 0.
  dx_size greatest_block;       ///< The size, in Bytes, of the greatest block allocated.
                                ///< Can be @a 0 even if Blocks are allocated as Blocks can have zero size.
                                ///< If no blocks are currently allocated, then the value is @a 0.
  dx_size number_of_blocks;     ///< The number of blocks currently allocated.
                                ///< The sum of the sizes, in Bytes, of all these blocks is the number of Bytes/KiloBytes allocated.
  dx_size number_of_bytes;      ///< The number of Bytes currently allocated.
  dx_size number_of_kilo_bytes; ///< The number of KiloBytes currently allocated (1 KiloByte = 1000 Bytes).
};

/// @brief Initialize a @a dx_allocator_statistics object.
/// @param self a pointer to an uninitialized a @a dx_allocator_statistics object.
/// @return The zero value on success. A non-zero value on failure.
int dx_allocator_statistics_initialize(dx_allocator_statistics *statistics);

/// @brief Uninitialize a @a dx_allocator_statistics object.
/// @param statistics a pointer to an uninitialized @a dx_allocator_statistics object
void dx_allocator_statistics_uninitialize(dx_allocator_statistics* statistics);

/// @brief Update memory statistics for an allocation.
/// @param statistics a pointer to an initialized @a (Spine_CollectorStatistics) object
/// @param number_of_bytes The number of Bytes that has been allocated.
/// @return The zero value on success. A non-zero value on failure.
int dx_allocator_Statistics_on_block_allocated(dx_allocator_statistics* statistics, dx_size number_of_bytes);

/// @brief Update memory statistics for a deallocation.
/// @param statistics a pointer to an initialized @a (Spine_CollectorStatistics) object
/// @param number_of_bytes The number of Bytes that has been deallocated.
/// @return The zero value on success. A non-zero value on failure.
int dx_allocator_statistics_on_block_deallocated(dx_allocator_statistics* statistics, dx_size number_of_bytes);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_MEMORY_H_INCLUDED
