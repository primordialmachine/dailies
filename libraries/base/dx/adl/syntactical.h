/// @file dx/adl/syntactical.h
/// @brief Syntactical functionality (aka "parsing") of Asset Description Language (ADL) files.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)

#if !defined(DX_ADL_SYNTACTICAL_H_INCLUDED)
#define DX_ADL_SYNTACTICAL_H_INCLUDED

#include "dx/adl/diagnostics.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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
DX_DECLARE_ENUMERATION_TYPE("dx.adl.word_kind", dx_adl_word_kind)

enum dx_adl_word_kind {
#define DEFINE(SYMBOL, STRING) \
  SYMBOL, 
#include "dx/adl/word_kind.i"
#undef DEFINE
};

dx_string* dx_adl_word_kind_to_string(dx_adl_word_kind self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.adl.word", dx_adl_word, dx_object)

static inline dx_adl_word* DX_ADL_WORD(void* p) {
  return (dx_adl_word*)p;
}

struct dx_adl_word {
  dx_object parent;
  dx_adl_word_kind kind;
  dx_string* text;
};

/// @brief Construct this word with the specified word kind and the specified word text.
/// @param self A pointer to this word.
/// @param kind The kind of the word.
/// @param text The text of the word.
int dx_adl_word_construct(dx_adl_word* self, dx_adl_word_kind kind, dx_string* text);

/// @brief Create this word with the specified word kind and the specified word text.
/// @param kind The kind of the word.
/// @param text The text of the word.
/// @return A pointer to the word on success. The null pointer on failure.
dx_adl_word* dx_adl_word_create(dx_adl_word_kind kind, dx_string* text);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A scanner used for scanning programs of the language "2023-06-01".
DX_DECLARE_OBJECT_TYPE("dx.adl.scanner",
                       dx_adl_scanner,
                       dx_object)

static inline dx_adl_scanner* DX_ADL_SCANNER(void* p) {
  return (dx_adl_scanner*)p;
}

struct dx_adl_scanner {
  dx_object _parent;
  /// @brief Pointer to the beginning of the first Byte of the input.
  char const* start;
  /// @brief Pointer to the end of the last Byte of the input.
  char const* end;
  /// @brief Pointer to the beginning of the current Byte.
  char const* current;

  /// @brief The text of the current word.
  dx_byte_array text;
  /// @brief The kind of the current word.
  dx_adl_word_kind kind;
};

/// @brief Construct this scanner with an empty input.
/// @param self A pointer to this scanner.
/// @success The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @failure This function has set the the error variable.
int dx_adl_scanner_construct(dx_adl_scanner* self);

/// @brief Create this scanner with an empty input.
/// @return A pointer to the scanner on success. The null pointer on failure.
/// @success The scanner was assigned the empty input and is in the start state w.r.t. the specified input.
/// @failure This function has set the the error variable.
dx_adl_scanner* dx_adl_scanner_create();

/// @brief Set the input to this scanner.
/// @param p A pointer to an array of @a l Bytes.
/// @param l The number of Bytes in the array pointed to by @a p.
/// @success The scanner was assigned the input and is in the start state w.r.t. the specified input.
/// @failure This function has set the the error variable.
int dx_adl_scanner_set(dx_adl_scanner* self, char const* p, size_t l);

/// @brief Compute the next word.
/// @param self A pointer to this scanner.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable.
int dx_adl_scanner_step(dx_adl_scanner* self);

/// @brief Get the text of the current token.
/// @param self A pointer to this scanner.
/// @return A pointer to an UTF-8 string on success. The null pointer on failure.
/// @failure This function has set the the error variable.
char const* dx_adl_scanner_get_word_text_bytes(dx_adl_scanner const* self);

/// @brief Get the length, in Bytes, of the text of the curren token.
/// @param self A pointer to this scanner.
/// @return The length, in Bytes, of the UTF-8 string. This may be @a 0. @a 0 is also returned on failure.
/// @failure This function has set the the error variable.
size_t dx_adl_scanner_get_word_text_number_of_bytes(dx_adl_scanner const* self);

/// @brief Get the type of the current token.
/// @param self A pointer to this scanner.
/// @return The type of the current token. #dx_adl_word_kind_error is returned on on failure.
/// @failure This function has set the the error variable.
dx_adl_word_kind dx_adl_scanner_get_word_kind(dx_adl_scanner const* self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("dx.adl.node_kind",
                            dx_adl_node_kind)

enum dx_adl_node_kind {
  dx_adl_node_kind_error = 0,

  dx_adl_node_kind_list,
  dx_adl_node_kind_map,
  dx_adl_node_kind_number,
  dx_adl_node_kind_string,
};

/// @brief An ADL node of a programs of the language "2023-06-01".
DX_DECLARE_OBJECT_TYPE("dx.adl.node",
                       dx_adl_node,
                       dx_object)

static inline dx_adl_node* DX_ADL_NODE(void* p) {
  return (dx_adl_node*)p;
}

struct dx_adl_node {
  dx_object _parent;
  dx_adl_node_kind kind;
  union {
    dx_pointer_array list;
    dx_pointer_hashmap map;
    dx_string* string;
    dx_string* number;
  };
};

/// @brief Construct this ADL node with the specified type.
/// @param self A pointer to this ADL node.
/// @param kind The kind of this ADL node.
/// @success The node was assigned default values for the specified type.
/// Default values are:
/// - #dx_adl_node_kind_empty: -
/// - #dx_adl_node_kind_string: the empty string
/// - #dx_adl_node_kind_number: the zero integer number
/// - #dx_adl_node_kind_map: the empty map
/// - #dx_adl_node_kind_list: the empty list
/// @failure This function has set the error variable.
int dx_adl_node_construct(dx_adl_node* self, dx_adl_node_kind kind);

/// @brief Create this ADL node with the specified type.
/// @param kind The kind of this ADL node.
/// @return A pointer to this ADL node. The null pointer on failure.
/// @success The node was assigned default values for the specified type.
/// See dx_adl_node_construct for details.
/// @failure This function has set the error variable.
dx_adl_node* dx_adl_node_create(dx_adl_node_kind kind);

/// @brief Get the node kind of this ADL node.
/// @param self A pointer to this ADL node.
/// @return The node kind. This may be #dx_node_kind_error. #dx_node_kind_error is also returned on failure.
/// @failure This function has set the the error variable.
dx_adl_node_kind dx_adl_node_get_kind(dx_adl_node const* self);

/// @brief Add or update the mapping from name to value in this ADL map node.
/// @param self A pointer to this ADL node.
/// @param name The name.
/// @param value The value.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_map.
int dx_adl_node_map_set(dx_adl_node* self, dx_string* name, dx_adl_node* value);

/// @brief Get a mapping from a name to a value in this ADL map node.
/// @param self A pointer to this ADL node.
/// @param name The name.
/// @return A pointer to the the value on success.
/// The null pointer on failure.
/// @failure This function has set the the error variable. In particular the following error codes are set
/// - #DX_NOT_FOUND no entry for the specified key was found
/// - #DX_INVALID_ARGUMENT @a self was a null pointer
/// - #DX_INVALID_ARGUMENT @a name was a null pointer
dx_adl_node* dx_adl_node_map_get(dx_adl_node const* self, dx_string* name);

/// @brief Append an ADL node to this ADL list node.
/// @param self A pointer to this ADL node.
/// @param value A pointer to the ADL node to append.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_list.
int dx_adl_node_list_append(dx_adl_node* self, dx_adl_node* value);

/// @brief Prepend an ADL node to this ADL list node.
/// @param self A pointer to this ADL node.
/// @param value A pointer to the ADL node to prepend.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_list.
int dx_adl_node_list_prepend(dx_adl_node* self, dx_adl_node* value);

/// @brief Insert an ADL node into this ADL list node.
/// @param self A pointer to this ADL node.
/// @param value A pointer to the ADL node to insert.
/// @param index The index at which to insert the ADL node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_list.
int dx_adl_node_list_insert(dx_adl_node* self, dx_adl_node* value, dx_size index);

/// @brief Get an ADL node at the specified index in this ADL list node.
/// @param self A pointer to this ADL node.
/// @param index The index at which to get the ADL node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a index is greater than or equal to the length of this ADL list node
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_list.
dx_adl_node* dx_adl_node_list_get(dx_adl_node* self, dx_size index);

/// @brief Get an ADL node at the specified index in this ADL list node.
/// @param self A pointer to this ADL node.
/// @param index The index at which to get the ADL node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a index is greater than or equal to the length of this ADL list node
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_list.
dx_size dx_adl_node_list_get_size(dx_adl_node* self);

/// @brief Get the string value of this ADL node.
/// @param self A pointer to this ADL node.
/// @return The string value on success. The null pointer on failure.
/// @success The caller acquired a reference to the returned dx_string object.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_string.
dx_string* dx_adl_node_get_string(dx_adl_node const* self);

/// @brief Set the string value of this ADL node.
/// @param self A pointer to this ADL node.
/// @param string A pointer to the string value.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a string is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_string.
/// @undefined The string is not well-formatted.
int dx_adl_node_set_string(dx_adl_node* self, dx_string* string);

/// @brief Get the number value of this ADL node.
/// @param self A pointer to this ADL node.
/// @return The number value on success. The null pointer on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_number.
dx_string* dx_adl_node_get_number(dx_adl_node const* self);

/// @brief Set the number value of this ADL node.
/// @param self A pointer to this ADL node.
/// @param number A pointer to the string value.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a number is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_adl_node_kind_number.
/// @undefined The string is not well-formatted.
int dx_adl_node_set_number(dx_adl_node* self, dx_string* number);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A parser used for parsing programs of the language "2023-06-01".
DX_DECLARE_OBJECT_TYPE("dx.adl.parser",
                       dx_adl_parser,
                       dx_object)

static inline dx_adl_parser* DX_ADL_PARSER(void* p) {
  return (dx_adl_parser*)p;
}

struct dx_adl_parser {
  dx_object _parent;
  /// @brief A pointer to the underlaying scanner.
  dx_adl_scanner* scanner;
  /// @brief A pointer to the underlaying diagnostics.
  dx_adl_diagnostics* diagnostics;
};

/// @brief Construct this parser with an empty input.
/// @param self A pointer to this parser.
/// @param scanner A pointer to the underlaying scanner.
/// @param diagnostics A pointer to the underlaying diagnostics.
/// @success The parser was assigned the empty input and is in the start state w.r.t. the specified input.
int dx_adl_parser_construct(dx_adl_parser* self, dx_adl_scanner* scanner, dx_adl_diagnostics* diagnostics);

/// @brief Create this parser with an empty input.
/// @param scanner A pointer to the underlaying scanner.
/// @param diagnostics A pointer to the underlaying diagnostics.
/// @return A pointer to the parser on success. The null pointer on failure.
/// @success The parser was assigned the empty input and is in the start state w.r.t. the specified input.
dx_adl_parser* dx_adl_parser_create(dx_adl_scanner* scanner, dx_adl_diagnostics* diagnostics);

/// @brief Set the input to this parser.
/// @param self A pointer to this parser.
/// @param p A pointer to an array of @a l Bytes.
/// @param l The number of Bytes in the array pointed to by @a p.
/// @success The parser was assigned the input and is in the start state w.r.t. the specified input.
int dx_adl_parser_set(dx_adl_parser* self, char const* p, size_t l);

/// @brief Get if the current word is of the specified word type.
/// @param self A pointer to this parser.
/// @param word_kind The word type.
/// @return @a true if the current word is of the specified word type. @a false if it is not. @a false is also returned on failure.
/// @failure This function has set the the error variable.
bool dx_adl_parser_is_word_kind(dx_adl_parser const* self, dx_adl_word_kind word_kind);

/// @brief Get the word type of the current word.
/// @param self A pointer to this parser.
/// @return The word type. This may be #dx_adl_word_kind_error. #dx_adl_word_kind_error is also returned on failure.
/// @failure This function has set the the error variable.
dx_adl_word_kind dx_adl_parser_get_word_kind(dx_adl_parser const* self);

/// @brief Run this parser.
/// @param self A pointer to this parser.
/// @return A pointer to the ADL node on success. The null pointer on failure.
dx_adl_node* dx_adl_parser_run(dx_adl_parser* self);

#if defined(DX_ADL_PARSER_WITH_TESTS) && DX_ADL_PARSER_WITH_TESTS

int dx_adl_parser_tests();

#endif // DX_ADL_PARSER_WITH_TESTS

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ADL_SYNTACTICAL_H_INCLUDED
