/// @file dx/ddl/parser.h
/// @brief Syntactical functionality (aka "parsing") of Asset Description Language (ADL) files.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DDL_PARSER_H_INCLUDED)
#define DX_DDL_PARSER_H_INCLUDED

#include "dx/ddl/diagnostics.h"

#include "dx/ddl/word_kind.h"
#include "dx/ddl/word.h"
#include "dx/ddl/scanner.h"
#include "dx/ddl/node_kind.h"
#include "dx/ddl/node.h"

/// @brief A parser used for parsing programs of the language "2023-06-01".
DX_DECLARE_OBJECT_TYPE("dx.ddl.parser",
                       dx_ddl_parser,
                       dx_object)

static inline dx_ddl_parser* DX_DDL_PARSER(void* p) {
  return (dx_ddl_parser*)p;
}

struct dx_ddl_parser {
  dx_object _parent;
  /// @brief A pointer to the underlaying scanner.
  dx_ddl_scanner* scanner;
  /// @brief A pointer to the underlaying diagnostics.
  dx_ddl_diagnostics* diagnostics;
};

/// @brief Construct this parser with an empty input.
/// @param self A pointer to this parser.
/// @param scanner A pointer to the underlaying scanner.
/// @param diagnostics A pointer to the underlaying diagnostics.
/// @success The parser was assigned the empty input and is in the start state w.r.t. the specified input.
int dx_ddl_parser_construct(dx_ddl_parser* self, dx_ddl_scanner* scanner, dx_ddl_diagnostics* diagnostics);

/// @brief Create this parser with an empty input.
/// @param scanner A pointer to the underlaying scanner.
/// @param diagnostics A pointer to the underlaying diagnostics.
/// @return A pointer to the parser on success. The null pointer on failure.
/// @success The parser was assigned the empty input and is in the start state w.r.t. the specified input.
dx_ddl_parser* dx_ddl_parser_create(dx_ddl_scanner* scanner, dx_ddl_diagnostics* diagnostics);

/// @brief Set the input to this parser.
/// @param self A pointer to this parser.
/// @param p A pointer to an array of @a l Bytes.
/// @param l The number of Bytes in the array pointed to by @a p.
/// @success The parser was assigned the input and is in the start state w.r.t. the specified input.
int dx_ddl_parser_set(dx_ddl_parser* self, char const* p, dx_size l);

/// @brief Get if the current word is of the specified word type.
/// @param self A pointer to this parser.
/// @param word_kind The word type.
/// @return @a true if the current word is of the specified word type. @a false if it is not. @a false is also returned on failure.
/// @failure This function has set the the error variable.
bool dx_ddl_parser_is_word_kind(dx_ddl_parser const* self, dx_ddl_word_kind word_kind);

/// @brief Get the word type of the current word.
/// @param self A pointer to this parser.
/// @return The word type. This may be #dx_ddl_word_kind_error. #dx_ddl_word_kind_error is also returned on failure.
/// @failure This function has set the the error variable.
dx_ddl_word_kind dx_ddl_parser_get_word_kind(dx_ddl_parser const* self);

/// @brief Run this parser.
/// @param self A pointer to this parser.
/// @return A pointer to the ADL node on success. The null pointer on failure.
dx_ddl_node* dx_ddl_parser_run(dx_ddl_parser* self);

#if defined(DX_DDL_PARSER_WITH_TESTS) && DX_DDL_PARSER_WITH_TESTS

int dx_ddl_parser_tests();

#endif // DX_DDL_PARSER_WITH_TESTS

#endif // DX_DDL_PARSER_H_INCLUDED
