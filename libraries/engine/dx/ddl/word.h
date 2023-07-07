/// @file dx/ddl/word.h
/// @brief Output of the lexical analyses of the Data Definition Language (DDL) programs.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
#if !defined(DX_DDL_WORD_H_INCLUDED)
#define DX_DDL_WORD_H_INCLUDED

#include "dx/ddl/word_kind.h"

/// @brief An word for programs of the Data Definition Language (DDL).
DX_DECLARE_OBJECT_TYPE("dx.ddl.word",
                       dx_ddl_word,
                       dx_object)

static inline dx_ddl_word* DX_DDL_WORD(void* p) {
  return (dx_ddl_word*)p;
}

struct dx_ddl_word {
  dx_object parent;
  dx_ddl_word_kind kind;
  dx_string* text;
};

/// @brief Construct this word with the specified word kind and the specified word text.
/// @param self A pointer to this word.
/// @param kind The kind of the word.
/// @param text The text of the word.
int dx_ddl_word_construct(dx_ddl_word* self, dx_ddl_word_kind kind, dx_string* text);

/// @brief Create this word with the specified word kind and the specified word text.
/// @param kind The kind of the word.
/// @param text The text of the word.
/// @return A pointer to the word on success. The null pointer on failure.
dx_ddl_word* dx_ddl_word_create(dx_ddl_word_kind kind, dx_string* text);

#endif // DX_DDL_WORD_H_INCLUDED
