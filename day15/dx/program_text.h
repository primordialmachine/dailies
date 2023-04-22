#if !defined(DX_PROGRAM_SOURCE_H_INCLUDED)
#define DX_PROGRAM_SOURCE_H_INCLUDED

#include "dx/core.h"

typedef enum dx_program_text_type {
  DX_PROGRAM_TEXT_TYPE_FRAGMENT = 1,
  DX_PROGRAM_TEXT_TYPE_VERTEX = 2,
  DX_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT = 3,
} dx_program_text_type;

/// @brief Representation of the source texts of a (parts of) programs.
/// The objects are tagged unions.
/// - If the tag #DX_PROGRAM_TEXT_TYPE_FRAGMENT or the tag #DX_PROGRAM_TEXT_TYPE_VERTEX is specified,
///   then dx_program_text::bytes and dx_program_text::number_of_bytes contain the (not necessarily well-formed) program text
///   of a fragment program and vertex program, respectively.
/// - If the tag #DX_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT is specified,
///   then dx_program_text::fragment_program_text and dx_program_text::vertex_program_text contain references to
///   dx_program_text objects of the type #DX_PROGRAM_TEXT_TYPE_FRAGMENT and #DX_PROGRAM_TEXT_TYPE_VERTEX, respectively.
typedef struct dx_program_text dx_program_text;

static inline dx_program_text* DX_PROGRAM_TEXT(void* p) {
  return (dx_program_text*)p;
}

struct dx_program_text {
  dx_object _parent;
  dx_program_text* parent;
  dx_program_text_type type;
  union {
    // For DX_PROGRAM_TEXT_TYPE_VERTEX or DX_PROGRAM_TEXT_TYPE_FRAGMENT
    dx_string* program_text;
    // For DX_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT
    struct {
      dx_program_text* vertex_program_text;
      dx_program_text* fragment_program_text;
    };
  };
};

/// @param program_text Pointer to this program text object.
/// @param type The type of the program text.
/// @param path The path to the file to load the program text from.
/// @return @a 0 on success. A non-zero value on failure.
int dx_program_text_construct_from_file(dx_program_text* program_text, dx_program_text_type type, dx_string* path);

/// @param program_text Pointer to this program text object.
/// @param vertex_program_text Pointer to a program text object of type DX_PROGRAM_SOURCE_TYPE_VERTEX.
/// @param fragment_program_text Pointer to a program text object of type DX_PROGRAM_SOURCE_TYPE_FRAGMENT.
/// @return @a 0 on success. A non-zero value on failure.
int dx_program_text_construct(dx_program_text* program_text, dx_program_text *vertex_program_text, dx_program_text* fragment_program_text);

void dx_program_text_destruct(dx_program_text* program_text);

dx_program_text* dx_program_text_create_from_file(dx_program_text_type type, dx_string* path);

dx_program_text* dx_program_text_create(dx_program_text *vertex_program_text, dx_program_text* fragment_program_text);

/// @brief Prefix the program with a define of the specified name.
/// @code
/// #define [name] (1)
/// @endcode
/// @param self A pointer to this program text.
/// @param name A pointer to the name.
/// @return The zero value on success. A non-zero value on failure.
int dx_program_text_add_define(dx_program_text* self, dx_string* name);

#endif // DX_PROGRAM_SOURCE_H_INCLUDED
