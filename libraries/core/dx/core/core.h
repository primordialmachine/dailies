#if !defined(DX_CORE_1_H_INCLUDED)
#define DX_CORE_1_H_INCLUDED

#include "dx/core/configuration.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// size_t, SIZE_MAX
#include <stddef.h>
// bool, true, false
#include <stdbool.h>
// uintx_t, UINTx_MAX
// intx_t, INTx_MIN, INTx_MAX
#include <inttypes.h>
// FLT_RADIX
#include <float.h>
// assert
#include <assert.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Macro causing a debug break if the expression evaluates to logically false.
/// @param expression The expression.
#define DX_DEBUG_ASSERT(expression) assert(expression)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief Symbolic constant for the Microsoft Windows operating system.
#define DX_OPERATING_SYSTEM_WINDOWS (1)

/// @ingroup core
/// @brief Symbolic constant for an Unix operating system.
#define DX_OPERATING_SYSTEM_UNIX (2)

/// @ingroup core
/// @brief Symbolic constant for Linux operating system.
#define DX_OPERATING_SYSTEM_LINUX (3)

/// @ingroup core
/// @brief Symbolic constant for a MacOS operating system.
#define DX_OPERATING_SYSTEM_MACOS (4)

/// @ingroup core
/// @brief Symbolic constant for a iOS operating system.
#define DX_OPERATING_SYSTEM_IOS (5)

/// @ingroup core
/// @brief Symbolic constant for a iOS simulator operating system.
#define DX_OPERATING_SYSTEM_IOSSIMULATOR (6)

/// @ingroup core
/// @brief Symbolic constant for the Cygwin/Microsoft Windows operating system.
#define DX_OPERATING_SYSTEM_CYGWIN (7)

/// @ingroup core
/// @brief Symbolic constant for the MSYS/Microsoft Windows operating system.
#define DX_OPERATING_SYSTEM_MSYS (8)

/// @ingroup core
/// @brief Symbolic constant for the MinGW/Microsoft Windows operating system.
#define DX_OPERATING_SYSTEM_MINGW (9)

/// @ingroup core
/// @brief Symbolic constant indicating the operating system.
/// It is defined to a value of one of the DX_OPERATING_SYSTEM_* symbolic constants.
#if defined(_WIN32)
  #define DX_OPERATING_SYSTEM DX_OPERATING_SYSTEM_WINDOWS
#else
  #error("operating system not supported")
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief Symbolic constant for the "MSVC" C compiler.
#define DX_COMPILER_C_MSVC (1)

/// @ingroup core
/// @brief Symbolic constant for the "GCC" C compiler.
#define DX_COMPILER_C_GCC (2)

/// @ingroup core
/// @brief Symbolic constant for the "CLANG" C compiler.
#define DX_COMPILER_C_CLANG (3)

/// @ingroup core
/// @brief Symbolic constant for the "MSVC" CPP compiler.
#define DX_COMPILER_CPP_MSVC (4)

/// @ingroup core
/// @brief Symbolic constant for the "GCC" CPP compiler.
#define DX_COMPILER_CPP_GCC (5)

/// @ingroup core
/// @brief Symbolic constant for the "CLANG" CPP compiler.
#define DX_COMPILER_CPP_CLANG (6)


/// @ingroup core
/// @brief Symbolic constant indicating the C compiler.
/// It is defined to a value of one of the DX_COMPILER_C_* symbolic constants.
#if defined(_MSC_VER)
  #define DX_COMPILER_C DX_COMPILER_C_MSVC
#elif defined(__GNUC__)
  #define DX_COMPILER_C DX_COMPILER_C_GNUC
#elif defined(__clang__)
  #define DX_COMPILER_C DX_COMPILER_C_CLANG
#else
  #error("C compiler not supported")
#endif


/// @ingroup core
/// @brief Symbolic constant indicating the CPP compiler.
/// It is defined to a value of one of the DX_COMPILER_CPP_* symbolic constants.
#if defined(_MSC_VER)
  #define DX_COMPILER_CPP DX_COMPILER_CPP_MSVC
#elif defined(__GNUC__)
  #define DX_COMPILER_CPP DX_COMPILER_CPP_GNUC
#elif defined(__clang__)
  #define DX_COMPILER_CPP DX_COMPILER_CPP_CLANG
#else
  #error("CPP compiler not supported")
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief Variable annotation indicating a thread local storage.
#if (DX_COMPILER_C == DX_COMPILER_C_GCC || DX_COMPILER_C == DX_COMPILER_C_CLANG) && !defined(DOXYGEN)
  #define DX_THREAD_LOCAL() __thread  /**< @hideinitializer */
#elif (DX_COMPILER_C == DX_COMPILER_C_MSVC) && !defined(DOXYGEN)
  #define DX_THREAD_LOCAL() __declspec(thread) /**< @hideinitializer */
#else
  #error("C compiler not supported")
#endif

/// @ingroup core
/// @brief Function annotation indicating that a function never returns.
/// @details
/// This function annotation indicates that a function never returns.
/// If it returns, it only returns by raising an exception or by calling `longjmp`.
#if (DX_COMPILER_C == DX_COMPILER_C_GCC || DX_COMPILER_C == DX_COMPILER_C_CLANG) && !defined(DOXYGEN)
  #define DX_NO_RETURN() __attribute__ ((noreturn)) /**< @hideinitializer */
#elif (DX_COMPILER_C == DX_COMPILER_C_MSVC) && !defined(DOXYGEN)
  #define DX_NO_RETURN() __declspec(noreturn) /**< @hideinitializer */
#else
  #error("C compiler not supported")
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
#if (DX_COMPILER_C == DX_COMPILER_C_GCC || DX_COMPILER_C == DX_COMPILER_C_CLANG) && !defined(DOXYGEN)
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
#if (DX_COMPILER_C == DX_COMPILER_C_GCC || DX_COMPILER_C == DX_COMPILER_C_CLANG) && !defined(DOXYGEN)
  #define DX_UNLIKELY(expression) (__builtin_expect((expression) ? 1 : 0, 0)) /**< @hideinitializer */
#else
  #define DX_UNLIKELY(expression) (expression) /**< @hideinitializer */
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core
/// @brief An alias for the <code>void</code> C type.
typedef void dx_void;

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
/// @brief Symbolic constant for a dx_error value indicating a failure because of a lexical error.
#define DX_LEXICAL_ERROR (7)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a syntactical error.
#define DX_SYNTACTICAL_ERROR (8)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of a semantical error.
#define DX_SEMANTICAL_ERROR (9)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because something was not found.
#define DX_NOT_FOUND (10)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because something exists.
#define DX_EXISTS (11)

/// @ingroup core
/// @brief Symbolic constant for a dx_error value indicating a failure because of an overflow.
#define DX_OVERFLOW (12)

/// @ingroup core
/// @brief Get the value of the error variable.
/// @return The value.
dx_error dx_get_error();

/// @ingroup core
/// @brief Set the value of the error variable.
/// @param error The value.
void dx_set_error(dx_error error);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief This must be aligned to 64-bit boundaries on 64-bit systems and to 32-bit boundaries on 32-bit systems.
#if defined(_WIN64)
  typedef int64_t dx_reference_counter;
#else
  typedef int32_t dx_reference_counter;
#endif

// @return The resulting incremented value.
dx_reference_counter dx_reference_counter_increment(dx_reference_counter* reference_counter);

// @return The resulting decremented value.
dx_reference_counter dx_reference_counter_decrement(dx_reference_counter* reference_counter);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Write an utf-8 string to standard output.
/// @warning There is no guarantee that the log message is written.
/// @param p A pointer to an array of @a n Bytes.
/// @param n The length of the array.
/// @pre
/// - @a p points to an array of at least @a n Bytes and
/// - the first @a n Bytes of that array are a valid utf-8 string
/// @pre
/// - utf8 is supported by standard output
void dx_log(char const *p, dx_size n);

/// @{
/// @brief Write a number to standard output.
/// @warning There is no guarantee that the log message is written.
/// @param value The number.

void dx_log_i64(dx_i64 value);

void dx_log_i32(dx_i32 value);

void dx_log_i16(dx_i16 value);

void dx_log_i8(dx_i8 value);

void dx_log_n64(dx_n64 value);

void dx_log_n32(dx_n32 value);

void dx_log_n16(dx_n16 value);

void dx_log_n8(dx_n8 value);

/// @}

/// @brief Write a pointer to standard output.
/// @warning There is no guarantee that the log message is written.
/// @param p The pointer.
void dx_log_p(void const* p);

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
dx_size dx_combine_hash(dx_size x, dx_size y);

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

#endif // DX_CORE_1_H_INCLUDED
