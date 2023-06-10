#if !defined(DX_CONFIGURATION_H_INCLUDED)
#define DX_CONFIGURATION_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_CONFIGURATION_VISUALS_DIRECT3D (1)

#define DX_CONFIGURATION_VISUALS_OPENGL (2)

#define DX_CONFIGURATION_VISUALS_VULKAN3D (3)

#define DX_CONFIGURATION_VISUALS DX_CONFIGURATION_VISUALS_OPENGL

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined an evaluating to @a 1 and if compiled in debug mode,
/// then "magic bytes" memory corruption detection pattern is available for dx_object.
/// A macro DX_DEBUG_CHECK_MAGIC_BYTES is provided.
/// If this function is invoked on a pointer that does not point to a valid dx_object object,
/// then there is a good chance (but only a chance) that this function will cause a debug break.
#define DX_OBJECT_WITH_MAGIC_BYTES (0)

/// If defined and evaluating to @a 1,
/// then atomic reference counting is enabled.
/// Otherwise it is disabled, that is, non-atomic reference counting is performed.
#define DX_ATOMIC_REFERENCE_COUNTING_ENABLED (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// then a function dx_math_tests() is provided
/// which performs tests on functionality provided by "dx/core/math.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_MATH_WITH_TESTS (1)

/// If defined and evaluating to @a 1,
/// the a function dx_safe_add_nx_tests() is provided
/// which performs tests on functionality provided by dx/core/safe_add_nx.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_SAFE_ADD_NX_WITH_TESTS (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// the a function dx_safe_mul_nx_tests() is provided
/// which performs tests on functionality provided by dx/core/safe_mul_nx.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_SAFE_MUL_NX_WITH_TESTS (1)

// If defined and evaluating to @a 1, then dx_mul_n8 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_N8_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_n16 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_N16_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_n32 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_N32_ASM (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// the a function dx_safe_mul_ix_tests() is provided
/// which performs tests on functionality provided by dx/core/safe_mul_ix.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_SAFE_MUL_IX_WITH_TESTS (1)

// If defined and evaluating to @a 1, then dx_mul_i8 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_I8_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_i16 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_I16_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_i32 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is preferred so keep this enabled.
#define DX_MUL_I32_ASM (1)

// If defined and evaluating to @a 1, then dx_mul_i64 uses an assembler implementation and otherwise a pure C implementation.
// Currently, the assember implementation is the only implementation so keep this enabled.
#define DX_MUL_I64_ASM (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// then a function dx_adl_parser_tests() is provided
/// which performs tests on functionality provided by "dx/asset/reader.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_ADL_PARSER_WITH_TESTS (1)

#endif // DX_CONFIGURATION_H_INCLUDED
