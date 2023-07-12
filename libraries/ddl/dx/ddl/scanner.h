/// @file dx/ddl/scanner.h
/// @brief Lexical analyser (aka "scanner") of Data Definition Language (DDL) programs.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DDL_SCANNER_H_INCLUDED)
#define DX_DDL_SCANNER_H_INCLUDED

#include "dx/ddl/diagnostics.h"
#include "dx/ddl/word_kind.h"
#include "dX/ddl/word.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A scanner used for scanning programs of the Data Definition Language (DDL).
DX_DECLARE_OBJECT_TYPE("dx.ddl.scanner",
                       dx_ddl_scanner,
                       dx_object)

static inline dx_ddl_scanner* DX_DDL_SCANNER(void* p) {
  return (dx_ddl_scanner*)p;
}

struct dx_ddl_scanner {
  dx_object _parent;

  /// @brief A pointer to the diagnostics used by this scanner.
  dx_ddl_diagnostics* diagnostics;

  /// @brief Pointer to the beginning of the first Byte of the input.
  char const* start;
  /// @brief Pointer to the end of the last Byte of the input.
  char const* end;
  /// @brief Pointer to the beginning of the current Byte.
  char const* current;

  /// @brief The text of the current word.
  dx_byte_array text;
  /// @brief The kind of the current word.
  dx_ddl_word_kind kind;
};

/// @brief Construct this scanner with an empty input.
/// @param self A pointer to this scanner.
/// @param diagnostics A pointer to the diagnostics used by this scanner.
/// @success The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @failure This function has set the the error variable.
int dx_ddl_scanner_construct(dx_ddl_scanner* self, dx_ddl_diagnostics* diagnostics);

/// @brief Create this scanner with an empty input.
/// @param diagnostics A pointer to the diagnostics used by this scanner.
/// @return A pointer to the scanner on success. The null pointer on failure.
/// @success The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @failure This function has set the the error variable.
dx_ddl_scanner* dx_ddl_scanner_create(dx_ddl_diagnostics* diagnostics);

/// @brief Set the input to this scanner.
/// @param p A pointer to an array of @a l Bytes.
/// @param l The number of Bytes in the array pointed to by @a p.
/// @success The scanner was assigned the input and is in the start state w.r.t. the specified input.
/// @failure This function has set the the error variable.
int dx_ddl_scanner_set(dx_ddl_scanner* self, char const* p, dx_size l);

/// @brief Compute the next word.
/// @param self A pointer to this scanner.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable.
int dx_ddl_scanner_step(dx_ddl_scanner* self);

/// @brief Get the text of the current token.
/// @param self A pointer to this scanner.
/// @return A pointer to an UTF-8 string on success. The null pointer on failure.
/// @failure This function has set the the error variable.
char const* dx_ddl_scanner_get_word_text_bytes(dx_ddl_scanner const* self);

/// @brief Get the length, in Bytes, of the text of the curren token.
/// @param self A pointer to this scanner.
/// @return The length, in Bytes, of the UTF-8 string. This may be @a 0. @a 0 is also returned on failure.
/// @failure This function has set the the error variable.
dx_size dx_ddl_scanner_get_word_text_number_of_bytes(dx_ddl_scanner const* self);

/// @brief Get the type of the current token.
/// @param self A pointer to this scanner.
/// @return The type of the current token. #dx_ddl_word_kind_error is returned on on failure.
/// @failure This function has set the the error variable.
dx_ddl_word_kind dx_ddl_scanner_get_word_kind(dx_ddl_scanner const* self);

#endif // DX_DDL_SCANNER_H_INCLUDED
