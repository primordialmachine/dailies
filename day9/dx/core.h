#if !defined(DX_CORE_H_INCLUDED)
#define DX_CORE_H_INCLUDED

// size_t, SIZE_MAX
#include <stddef.h>
// uintx_t, UINTx_MAX
// intx_t, INTx_MIN, INTx_MAX
#include <inttypes.h>
// bool, true, false
#include <stdbool.h>
// FLT_RADIX
#include <float.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief Symbolic constant for the "MSVC" compiler.
#define DX_COMPILER_MSVC (1)

/// @ingroup core
/// @brief Symbolic constant for the "GCC" compiler.
#define DX_COMPILER_GCC (2)

/// @ingroup core
/// @brief Symbolic constant for the "CLANG" compiler.
#define DX_COMPILER_CLANG (3)

/// @ingroup core
/// @brief Symbolic constant indicating the compiler.
/// It is defined to one of #DX_COMPILER_MSVC, #DX_COMPILERE_GCC, or #DX_COMPILER_CLANG.
#if defined(_MSC_VER)
  #define DX_COMPILER DX_COMPILER_MSVC
#elif defined(__GNUC__)
  #define DX_COMPILER DX_COMPILER_GNUC
#elif defined(__clang__)
  #define DX_COMPILER DX_COMPILER_CLANG
#else
  #error("compiler not supported")
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief Function annotation indicating that a function never returns.
/// @details
/// This function annotation indicates that a function never returns.
/// If it returns, it only returns by raising an exception or by calling `longjmp`.
#if (DX_COMPILER == DX_COMPILER_GCC || DX_COMPILER == DX_COMPILER_CLANG) && defined(DOXYGEN)
  #define DX_NO_RETURN() __attribute__ ((noreturn)) /**< @hideinitializer */
#elif (DX_COMPILER == DX_COMPILER_MSVC) && !defined(DOXYGEN)
  #define DX_NO_RETURN() __declspec(noreturn) /**< @hideinitializer */
#else
  #define DX_NO_RETURN()
#endif

/// @ingroup core
/// @brief Expression annotation providing branch prediction information, indicating an expression is likely to evaluate to logically true.
/// @details
/// This expression annotation indicates that the expression `expression` is likely to evaluate to logically false.
///
/// For example, given contemporary compilers and environments, it is likely that `malloc` for a small size of memory
/// is very likely to succeed. Hence, in the following code snipped, we tell tell the compiler that the test for success
/// of such a call to `malloc` is likely to to evaluate to logically true and the code in branch `(A)` is likely to
/// be executed whilst consequently the code in branch `(X)` is not likely to be execute.
/// ```
/// char *p = malloc(sizeof(char) * 1024);
/// if (DX_LIKELY(p)) {
///   /* (A) Foaming with joy, continue execution. */
/// } else {
///   /* (X) In the heart with pain, abort execution. */
/// }
/// ```
#if (DX_COMPILER == DX_COMPILER_GCC || DX_COMPILER == DX_COMPILER_CLANG) && defined(DOXYGEN)
  #define DX_LIKELY(expression) (__builtin_expect((expression) ? 1 : 0, 1)) /**< @hideinitializer */
#else
  #define DX_LIKELY(expression) (expression) /**< @hideinitializer */
#endif

/// @ingroup core
/// @brief Expression annotation providing branch prediction information, indicating an expression is likely to evaluate to logically false.
/// @details
/// This expression annotation indicates that the expression `expression` is likely to evaluate to logically false.
///
/// For example, given contemporary compilers and environments, it is likely that `malloc` for a small size of memory
/// is very likely to succeed. Hence, in the following code snipped, we tell tell the compiler that the test for failure
/// of such a call to `malloc` is likely to to evaluate to logically false and the code in branch `(A)` is likely to
/// be executed whilst consequently the code in branch `(X)` is not likely to be execute.
/// ```
/// char *p = malloc(sizeof(char) * 1024);
/// if (DX_UNLIKELY(!p)) {
///   /* (X) In the heart with pain, abort execution. */
/// } else {
///   /* (A) Foaming with joy, continue execution. */
/// }
/// ```
#if (DX_COMPILER == DX_COMPILER_GCC || DX_COMPILER == DX_COMPILER_CLANG) && defined(DOXYGEN)
  #define DX_UNLIKELY(expression) (__builtin_expect((expression) ? 1 : 0, 0)) /**< @hideinitializer */
#else
  #define DX_UNLIKELY(expression) (expression) /**< @hideinitializer */
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief A value that can assume two values indicating logically true and logically false.
/// @remark An alias for <code>bool</code>.
typedef bool dx_bool;
/// @ingroup core
/// @brief The value of type @a dx_bool indicating logically true.
#define DX_TRUE (true)
/// @ingroup core
/// @brief The value of type @a dx_bool indicating logically false.
#define DX_FALSE (false)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief A natural value of at least 32 bits width.
/// @remark An alias for <code>size_t</code>.
typedef size_t dx_size;
/// @ingroup core
/// @brief The least value of the type @a dx_size.
#define DX_SIZE_LEAST (0)
/// @ingroup core
/// @brief The greatest value of the type @a dx_size.
#define DX_SIZE_GREATEST (SIZE_MAX)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief A natural value of 8 bits width.
/// @remark An alias for <code>uint8_t</code>.
typedef uint8_t dx_n8;
/// @ingroup core
/// @brief The least value of the type @a dx_n8.
#define DX_N8_LEAST (0)
/// @ingroup core
/// @brief The greatest value of the type @a dx_n8.
#define DX_N8_GREATEST (UINT8_MAX)

/// @ingroup core
/// @brief A natural value of 16 bits width.
/// @remark An alias for <code>uint16_t</code>.
typedef uint16_t dx_n16;
/// @ingroup core
/// @brief The least value of the type @a dx_n16.
#define DX_N16_LEAST (0)
/// @ingroup core
/// @brief The greatest value of the type @a dx_n16.
#define DX_N16_GREATEST (UINT16_MAX)

/// @ingroup core
/// @brief A natural value of 32 bits width.
/// @remark An alias for <code>uint32_t</code>.
typedef uint32_t dx_n32;
/// @ingroup core
/// @brief The least value of the type @a dx_n32.
#define DX_N32_LEAST (0)
/// @ingroup core
/// @brief The greatest value of the type @a dx_n32.
#define DX_N32_GREATEST (UINT32_MAX)

/// @ingroup core
/// @brief A natural value of 64 bits width.
/// @remark An alias for <code>uint64_t</code>.
typedef uint64_t dx_n64;
/// @ingroup core
/// @brief The least value of the type @a dx_n64.
#define DX_N64_LEAST (0)
/// @ingroup core
/// @brief The greatest value of the type @a dx_n64.
#define DX_N64_GREATEST (UINT64_MAX)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief An integer value of 8 bits width.
/// @remark An alias for <code>int8_t</code>.
typedef int8_t dx_i8;
/// @ingroup core
/// @brief The least value of the type @a dx_i8.
#define DX_I8_LEAST (INT8_MIN)
/// @ingroup core
/// @brief The greatest value of the type @a dx_i8.
#define DX_I8_GREATEST (INT8_MAX)

/// @ingroup core
/// @brief An integer value of 16 bits width.
/// @remark An alias for <code>int16_t</code>.
typedef int16_t dx_i16;
/// @ingroup core
/// @brief The least value of the type @a dx_i16.
#define DX_I16_LEAST (INT16_MIN)
/// @ingroup core
/// @brief The greatest value of the type @a dx_i16.
#define DX_I16_GREATEST (INT16_MAX)

/// @ingroup core
/// @brief An integer value of 32 bits width.
/// @remark An alias for <code>int32_t</code>.
typedef int32_t dx_i32;
/// @ingroup core
/// @brief The least value of the type @a dx_i32.
#define DX_I32_LEAST (INT32_MIN)
/// @ingroup core
/// @brief The greatest value of the type @a dx_i32.
#define DX_I32_GREATEST (INT32_MAX)

/// @ingroup core
/// @brief An integer value of 64 bits width.
/// @remark An alias for <code>int64_t</code>.
typedef int64_t dx_i64;
/// @ingroup core
/// @brief The least value of the type @a dx_i64.
#define DX_I64_LEAST (INT64_MIN)
/// @ingroup core
/// @brief The greatest value of the type @a dx_i64.
#define DX_I64_GREATEST (INT64_MAX)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief A IEEE 754 32 bit floating point value.
/// @remark An alias for <code>float</code>.
typedef float dx_f32;
/// @ingroup core
/// @brief The least value of the type @a dx_f32.
#define DX_F32_LEAST (-FLT_MAX)
/// @ingroup core
/// @brief The greatest value of the type @a dx_f32.
#define DX_F32_GREATEST (FLT_MAX)
/// @ingroup core
/// @brief The "positive infinity" value of type @a dx_f32.
#define DX_F32_POSITIVE_INFINITY (+INFINITY)
/// @ingroup core
/// @brief The "negative infinity" value of type @a dx_f32.
#define DX_F32_NEGATIVE_INFINITY (-INFINITY)
/// @ingroup core
/// @brief The "no number" value of type @a dx_f32.
#define DX_F32_NO_NUMBER (NAN)
/// @ingroup core
/// @brief The greatest integer such that <code>10</code> raised to that power is a normalized @a dx_f32 value.
#define DX_F32_GREATEST_10_EXP (FLT_MAX_10_EXP)
/// @ingroup core
/// @brief The least integer such that <code>10</code> raised to that power is it a normalized @a dx_f32 value.
#define DX_F32_LEAST_10_EXP (FLT_MIN_10_EXP)
// All machines we're aware of define this to 2 except of IBM 360 and derivatives.
#if defined(FLT_RADIX) && 2 == FLT_RADIX
  /// @ingroup core
  /// @brief The greatest integer such that <code>2</code> raised to that power is a normalized @a dx_f32 value.
  #define DX_F32_GREATEST_2_EXP (FLT_MAX_EXP)
  /// @ingroup core
  /// @brief The least integer such that <code>2</code> raised to that power is a normalized @a dx_f32 value.
  #define DX_F32_LEAST_2_EXP (FLT_MIN_EXP)
#else
  #error("target environment not supported")
#endif

/// @ingroup core
/// @brief A IEEE 754 64 bit floating point value.
/// @remark An alias for <code>double</code>.
typedef double dx_f64;
/// @ingroup core
/// @brief The least value of the type @a dx_f64.
#define DX_F64_LEAST (-DLB_MAX)
/// @ingroup core
/// @brief The greatest value of the type @a dx_f64.
#define DX_F64_GREATEST (DBL_MAX)
/// @ingroup core
/// @brief The "positive infinity" value of type @a dx_f64.
#define DX_F64_POSITIVE_INFINITY (+INFINITY)
/// @ingroup core
/// @brief The "negative infinity" value of type @a dx_f64.
#define DX_F64_NEGATIVE_INFINITY (-INFINITY)
/// @ingroup core
/// @brief The "no number" value of type @a dx_f64.
#define DX_F64_NO_NUMBER (NAN)
/// @ingroup core
/// @brief The greatest integer such that <code>10</code> raised to that power is a normalized @a dx_f64 value.
#define DX_F64_GREATEST_10_EXP (DBL_MAX_10_EXP)
/// @ingroup core
/// @brief The least integer such that <code>10</code> raised to that power is a normalized @a dx_f64 value.
#define DX_F64_LEAST_10_EXP (DBL_MIN_10_EXP)
// All machines we're aware of define this to 2 except of IBM 360 and derivatives.
#if defined(FLT_RADIX) && 2 == FLT_RADIX
  /// @ingroup core
  /// @brief The greatest integer such that <code>2</code> raised to that power is a normalized @a dx_f64 value.
  #define DX_F64_GREATEST_2_EXP (DBL_MAX_EXP)
  /// @ingroup core
  /// @brief The least integer such that <code>2</code> raised to that power is a normalized @a dx_f64 value.
  #define DX_F64_LEAST_2_EXP (DBL_MIN_EXP)
#else
  #error("target environment not supported")
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief The integer type of values indicating error status.
/// The special value DX_NO_ERROR evaluates to @a 0 and indicates the absence of an error.
typedef int dx_error;

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating the absence of an error.
/// The dx_error value indicating the absence of an error is the zero value.
#define DX_NO_ERROR (0)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a failed allocation.
/// This value is guaranteed to be a non-zero value.
#define DX_ALLOCATION_FAILED (1)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of an invalid argument.
/// This value is guaranteed to be a non-zero value.
#define DX_INVALID_ARGUMENT (2)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of an invalid operation.
/// This value is guaranteed to be a non-zero value.
#define DX_INVALID_OPERATION (3)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a failed environment.
/// This value is guaranteed to be a non-zero value.
#define DX_ENVIRONMENT_FAILED (4)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a failed conversion.
/// This value is guaranteed to be a non-zero value.
#define DX_CONVERSION_FAILED (5)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a failed decoding.
/// This value is guaranteed to be a non-zero value.
#define DX_DECODING_FAILED (6)

/// @ingroup core
/// @brief Get the value of the error variable.
/// @return The value.
dx_error dx_get_error();

/// @ingroup core
/// @brief Set the value of the error variable.
/// @param error The value.
void dx_set_error(dx_error error);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef int64_t dx_reference_counter;

// @return The resulting incremented value.
dx_reference_counter dx_reference_counter_increment(dx_reference_counter* reference_counter);

// @return The resulting decremented value.
dx_reference_counter dx_reference_counter_decrement(dx_reference_counter* reference_counter);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// The base of non-POD objects.
typedef struct dx_object dx_object;

/// @brief
/// Unconditionally cast a <code>void</code> pointer into a <code>dx_object</code> pointer.
/// @param p The <code>void</code> pointer.
/// @return The <code>dx_object</code> pointer.
static inline dx_object* DX_OBJECT(void *p) {
  return (dx_object*)p;
}

struct dx_object {
  /// @brief The reference count of the object.
  dx_reference_counter reference_count;
  /// @brief A pointer to the destructor of the object or a null pointer.
  void (*destruct)(dx_object* object);
};

/// @brief
/// Allocate a dx_object.
/// @param size
/// The size, in Bytes, of the object. Must be greater than or equal to <code>sizeof(dx_object)</code>.
/// @return
/// A pointer to the object on success. A null pointer on failure.
/// @post
/// On success, a pointer to dx_object object is returned.
/// The object is initialized such that <code>dx_object::destructor</code> is assigned a null pointer and <code>dx_object::reference_count</code> is assigned @a 1.
dx_object* dx_object_alloc(size_t size);

/// @brief
/// Increment the reference count of a dx_object object by @a 1.
/// @param object
/// A pointer to the dx_object object.
void dx_object_reference(dx_object *object);

/// @brief
/// Increment the reference count of a dx_object object by @a 1.
/// @param object
/// A pointer to the dx_object object.
void dx_object_unreference(dx_object* object);

static inline void DX_REFERENCE(void *p) {
  dx_object_reference(DX_OBJECT(p));
}

static inline void DX_UNREFERENCE(void *p) {
  dx_object_unreference(DX_OBJECT(p));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// @brief Write an utf-8 string to standard output.
// @warning There is no guarantee that the log message is written.
// @param p A pointer to an array of @a n Bytes.
// @param n The length of the array.
// @pre
// - @a p does poins to an array of at least @a n Bytes and
// - the first @a n Bytes of that array are a valid utf-8 string
// @pre
// - utf8 us supported by standard output
void dx_log(char const *p, size_t n);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MSG_TYPE_UNDETERMINED (0)

#define DX_MSG_TYPE_EMIT (1)

#define DX_MSG_TYPE_QUIT (2)

// The opaque type of a message.
typedef struct dx_msg dx_msg;

static inline dx_msg* DX_MSG(void *p) {
  return (dx_msg*)p;
}

uint32_t dx_msg_get_flags(dx_msg const* msg);

int dx_msg_construct(dx_msg* msg);

/// @brief Destruct this message.
/// @param msg A pointer to this message.
void dx_msg_destruct(dx_msg* msg);

struct dx_msg {
  dx_object _parent;
  uint32_t flags;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of an "emit" message.
typedef struct dx_emit_msg dx_emit_msg;

static inline dx_emit_msg* DX_EMIT_MSG(void *p) {
  return (dx_emit_msg*)p;
}

int dx_emit_msg_construct(dx_emit_msg* emit_msg, char const *p, size_t n);

void dx_emit_msg_destruct(dx_emit_msg* emit_msg);

// Create an "emit" message.
// @param p Pointer to an utf-8 string of @a n Bytes.
// @param n the length of the utf-8 string in Bytes pointed to by @a p.
// @return A pointer to the message on success. The null pointer on failure.
dx_emit_msg* dx_emit_msg_create(char const *p, size_t n);

int dx_emit_msg_get(dx_emit_msg* emit_msg, char const** p, size_t* n);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of an "quit" message.
typedef struct dx_quit_msg dx_quit_msg;

static inline dx_quit_msg* DX_QUIT_MSG(void *p) {
  return (dx_quit_msg*)p;
}

int dx_quit_msg_construct(dx_quit_msg* quit_msg);

void dx_quit_msg_destruct(dx_quit_msg* emit_msg);

// Create a "quit" message
// @return pointer to the message on success. null pointer on failure
dx_quit_msg* dx_quit_msg_create();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of a message queue.
typedef struct dx_msg_queue dx_msg_queue;

// Push a message on the message queue.
// @post
// On success:
// The message was added to the queue and the queue acquired a reference to the message.
// @return @a 0 on success. A non-zero value on failure.
int dx_msg_queue_push(dx_msg_queue* msg_queue, dx_msg* msg);

// Pop a message from the message queue.
// @post
// On success:
// <code>*msg</code> was assigned a pointer to the message if the queue is not empty.
// The reference acquired to the message when it was added to the message queue was transferred to the caller.
// It was assigned the null pointer otherwise.
// On failure:
// <code>msg</code> is not dereferenced.
// @return
// @a 0 on success. A non-zero value on failure
int dx_msg_queue_pop(dx_msg_queue* msg_queue, dx_msg** msg);

// Create a message queue.
// @return pointer to the message queue on success. null pointer on failure.
dx_msg_queue *dx_msg_queue_create();

// Destroy a message queue.
// The message queue relinquishes all references to messages contained in the queue.
void dx_msg_queue_destroy(dx_msg_queue *msg_queue);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @{
/// @ingroup core
/// @brief Compare two floating-point values.
/// @param a,b The floating-point values to compare.
/// @param epsilon An epsilon used by some comparison methods.
/// Must not be nan.
/// @param method
/// Denotes the method used for comparison.
/// The following methods are available:
/// - @a 1: <code>a == b direct comparison</code>
/// - @a 2: <code>|a-b| &leq; epsilon</code>
/// - @a 3: <code>|a-b| &leq; epsilon * max(|a|,|b|)</code>
/// In either method, a and b are considered equal if they are both nan
/// @return @a true if @a a and @a b are equal.
/// @a false if they are not equal or an error occurred.
bool dx_almost_equal_f32(float a, float b, int method, float epsilon);
bool dx_almost_equal_f64(double a, double b, int method, double epsilon);
/// @}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup os
/// @brief Get the page size.
/// @return The page size on success. @a 0 on failure.
/// @failure This function has set the error variable.
size_t dx_os_get_page_size();

/// @ingroup os
/// @brief Get the number of CPU cores.
/// @return The number of cores on success. @a 0 on failure.
/// @failure This function has set the error variable.
size_t dx_os_get_number_of_cores();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup filesystem
/// @brief An enumeration of file access modes to files.
/// @remarks The enumeration elements can be combined.
typedef enum DX_FILE_ACCESS_MODE {

  /// @brief Read access.
  /// @remark Can be combined with DX_FILE_ACCESS_MODE_WRITE.
  DX_FILE_ACCESS_MODE_READ = 1,

  /// @brief Write access.
  /// @remark Can be combined with DX_FILE_ACCESS_MODE_READ.
  DX_FILE_ACCESS_MODE_WRITE = 2,

  /// @brief Read and write access.
  /// @remark Alias for @code{DX_FILE_ACCESS_MODE_READ|DX_FILE_ACCESS_MODE_WRITE}.
  /// Alias for @code{DX_FILE_ACCESS_MODE_WRITE_READ}.
  DX_FILE_ACCESS_MODE_READ_WRITE = DX_FILE_ACCESS_MODE_READ | DX_FILE_ACCESS_MODE_WRITE,

  /// @brief Write and read access.
  /// @remark Alias for @code{DX_FILE_ACCESS_MODE_WRITE|DX_FILE_ACCESS_MODE_READ}.
  /// Alias for @code{DX_FILE_ACCESS_MODE_READ_WRITE}.
  DX_FILE_ACCESS_MODE_WRITE_READ = DX_FILE_ACCESS_MODE_WRITE | DX_FILE_ACCESS_MODE_READ,

} DX_FILE_ACCESS_MODE;

/// @ingroup filesystem
/// @brief An enumeration of policies for opening a file in case of that the file exists.
/// @remarks The enumeration elements can not be combined.
typedef enum DX_EXISTING_FILE_POLICY {

  /// @brief Retain the file.
  DX_EXISTING_FILE_POLICY_RETAIN,

  /// @brief Truncate the file.
  DX_EXISTING_FILE_POLICY_TRUNCATE,

} DX_EXISTING_FILE_POLICY;

/// @ingroup filesystem
/// @brief An enumeration of policies for opening a file in case of that the file does not exist.
/// @remarks The enumeration elements can not be combined.
typedef enum DX_NON_EXISTING_FILE_POLICY {

  /// @brief Fail.
  DX_NON_EXISTING_FILE_POLICY_FAIL,

  /// @brief Create the file.
  DX_NON_EXISTING_FILE_POLICY_CREATE,

} DX_NON_EXISTING_FILE_POLICY;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief Compute the hash value of a pointer.
/// @param x The pointer.
/// @return The hash value.
dx_size dx_hash_pointer(void const* );

/// @ingroup core
/// @brief Compute the hash value of a <code>dx_bool</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_bool(dx_bool x);

/// @ingroup core
/// @brief Compute the hash value of an array of Bytes.
/// @param p A pointer to the array of Bytes.
/// @param n The length of the array.
/// @return The hash value.
dx_size dx_hash_bytes(void const* p, dx_size n);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_f32</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_f32(dx_f32 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_f64</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_f64(dx_f64 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_i8</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_i8(dx_i8 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_i16</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_i16(dx_i16 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_i32</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_i32(dx_i32 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_i64</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_i64(dx_i64 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_n8</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_n8(dx_n8 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_n16</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_n16(dx_n16 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_n32</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_n32(dx_n32 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_n64</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_n64(dx_n64 x);

/// @ingroup core
/// @brief Compute the hash value of an <code>dx_size</code> value.
/// @param x The value.
/// @return The hash value.
dx_size dx_hash_sz(dx_size x);

/// @ingroup core
/// @brief Combine two hash values.
/// @param x The first hash value.
/// @param y The second hash value.
/// @return The combination of the first hash value and the second hash value.
dx_size_dx_combine_hash(dx_size x, dx_size y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @{
/// @ingroup core
/// @brief Get if a floating point value is a subnormal value.
/// @param x The floating point value.
/// @return @a true if the floating point value is a subnormal value, @a false otherwise.
dx_bool dx_fp32_is_subnormal(dx_f32 x);
dx_bool dx_fp64_is_subnormal(dx_f64 x);
/// @}

/// @{
/// @ingroup core
/// @brief Get if a floating point value is a not a number value.
/// @param x The floating point value.
/// @return @a true if the floating point value is a not a number value, @a false otherwise.
dx_bool dx_fp32_is_nan(dx_f32 x);
dx_bool dx_fp64_is_nan(dx_f64 x);
/// @}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_H_INCLUDED
