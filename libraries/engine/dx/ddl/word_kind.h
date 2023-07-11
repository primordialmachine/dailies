/// @file dx/ddl/syntactical.h
/// @brief Enumeration of word kinds of the Data Definition Language (DDL).
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DDL_WORD_KIND_H_INCLUDED)
#define DX_DDL_WORD_KIND_H_INCLUDED

#include "dx/core.h"

/// @brief Enumeration of word kinds.
/// @details
/// The following section describes the lexicals of the asset description language.
///
/// # Notation
/// The description uses a form of an EBNF.
/// A word is a rule, a symbol, a choice, a group, a repetition, or a range.</br>
///
/// The grammar provides means to describe how words are formed from symbols.
/// The grammar consists of words and symbols.
/// A word has (unique) name in snake case (e.g., digit).
/// A word is defined by writing down its name, followed by <code>:=</code> followed by a symbol, a choice, a group, a repetition, or a range.
///
/// We denote symbols either by an escape sequence enclosed by single quotes (e.g., <code>'\t'</code> for horiziontal tabulation) by their UTF-8 code point (e.g,, U+0009 for horizontal tabulation).
///
/// Ranges of symbols are denoted by left square bracket followed by the symbol followed by a dash followed by the last symbol followed by a right square bracket (e.g., ['0'-'9'] for the digits from zero to nine).
///
/// A "group" is a left parenthesis followed by a category, a symbol, a choice, a group, a repetition, or a range.
/// A "choice" are any combination of categories, symbols, choices, groups, repetitions, or ranges separated by vertical bars.
///
/// An "option" is a category, a symbol, a group, or a range followed by a question mark.
/// A "repetition of zero or more occurences" is  a category, a symbol, a group, or a range followed by a star.
/// A "repetition of one or more occurences" is a category, a symbol, a group, or a range followed by a plus.
///
/// # Lexicals of the ADL
/// This section provides the EBNF of the lexical grammar of the asset description language (ADL).
///
/// ## whitespace
/// The word <code>whitespace</code> are
/// @code
/// whitespace := '\t' | ' '
/// @endcode
///
/// ## newline
/// The word <code>newline</code> are
/// @code
/// newline := '\\n' | '\\r' | '\\n\\r' | '\\r\\n'
/// @endcode
///
/// ## comments
/// The language supports both single-line comments and multi-line comments.
/// Single-line comments start with two consecutive slashes @code{//} and extend to the end of the line.
/// Multi-line comments start with a slash and star @code{/*} and end with a star and a slash @code{*/}.
/// @code{//} has no special meaning either comment.
/// @code{/*} and @code{*/} have no special meaning single-line comments.
/// Currently, the language does not provide means to enter @code{*/} multi-line comments.
/// Multi-line comments do not ntest.
/// 
/// ## digit
/// The word <code>digit</code> are
/// @code
/// digit := ['0'-'9']
/// @endcode
///
/// ## parentheses
/// The words <code>left_parenthesis</code> and <code>right_parenthesis</code>, respectively, are
/// @code
/// left_parenthesis := '('
/// right_parenthesis := ')'
/// @endcode
///
/// ## curly brackets
/// The words <code>left_curly_bracket</code> and <code>right_curly_bracket</code>, respectively, are
/// @code
/// left_curly_bracket := '{'
/// right_curly_bracket := '}'
/// @endcode
/// 
/// ## colon
/// The word <code>colon</code> is
/// @code
/// colon = ':'
/// @endcode
/// 
/// ## single quote
/// The word <code>single_quote</code> is
/// @code
/// single_quite = '\''
/// @endcode
/// 
/// ## double quote
/// The word <code>double_quote</code> is
/// @code
/// double_quote = '"'
/// @endcode
/// 
/// ## square brackets
/// The words <code>left_square_bracket</code> and <code>right_square_bracket</code>, respectively, are
/// @code
/// left_square_bracket := '['
/// right_square_bracket := ']'
/// @endcode
///
/// ## alphabetic
/// The terminal <code>alphabetic</code> contains the letters of the alphabet in small case and capital case.
/// @code
/// alphabetic := 'a'|'b'|'c'|'d'|'e'|'f'|'g'|'h'|'i'|'j'|'k'|'l'|'m'|'n'|'o'|'p'|'q'|'r'|'s'|'t'|'u'|'v'|'w'|'x'|'y'|'z'
///             | 'A'|'B'|'C'|'D'|'E'|'F'|'G'|'H'|'I'|'J'|'K'|'L'|'M'|'N'|'O'|'P'|'Q'|'R'|'S'|'T'|'U'|'V'|'W'|'X'|'Y'|'Z'
/// @endcode
///
/// ## alphanumeric
/// The terminal <code>alphanumeric</code> is reserved for future use.
///
/// ## comma
/// The word <code>comma</code> is the comma
/// @code
/// comma := ','
/// @endcode
///
/// ## underscore
/// The terminal <code>underscore</code> is the underscore
/// @code
/// underscore := '_'
/// @endcode
///
/// ## name
/// The non-terminal <code>name</code> is defined as
/// @code
/// name := {underscore}, alphabetic, {alphabetic|digit|underscore}
/// @endcode
///
/// ## number
/// The non-terminal <code>number</code> is defined as
/// @code
/// number := integer_number
///         | real_number
/// integer_number := ['+' | '-'], digit, {digit}
/// real_number := [ '+' | '-'], '.', digit, {digit}, [exponent]
///              | [ '+' | '-' ] digit, {digit} ['.', {digit}] [exponent]
/// exponent := ('e'|'E'), ['+' | '-'], digit, {digit}
/// @endcode
///
/// <b>string</b>
/// The word <code>string</code> is
/// @code
/// string := single_quoted_string
///         | double_quoted_string
/// double_quoted_string := double_quote, {NOT(newline | double_quote)}, double_quote
/// single_quoted_string := single_quote, {NOT(newline | single_quote)}, single_quote
/// @endcode
DX_DECLARE_ENUMERATION_TYPE("dx.ddl.word_kind",
                            dx_ddl_word_kind)

enum dx_ddl_word_kind {
#define DEFINE(SYMBOL, STRING) \
  SYMBOL,
#include "dx/ddl/word_kind.i"
#undef DEFINE
};

dx_string* dx_ddl_word_kind_to_string(dx_ddl_word_kind self);

#endif // DX_DDL_WORD_KIND_H_INCLUDED
