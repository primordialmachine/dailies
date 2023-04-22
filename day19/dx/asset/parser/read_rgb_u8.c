#include "dx/asset/parser/read_rgb_u8.h"

#include <string.h>

static inline bool _IS(dx_adl_parser* parser, dx_adl_word_type word_type, dx_string* word_text) {
  if (!dx_adl_parser_is_word_type(parser, word_type)) {
    return false;
  }
  if (word_text) {
    if (parser->scanner->text.size != word_text->number_of_bytes) {
      return false;
    }
    if (memcmp(parser->scanner->text.elements, word_text->bytes, word_text->number_of_bytes)) {
      return false;
    }
  }
  return true;
}

static inline int _EXPECT(dx_adl_parser* parser, dx_adl_word_type word_type, dx_string* word_text) {
  if (!_IS(parser, word_type, word_text)) {
    if (!dx_get_error()) {
      dx_set_error(DX_SYNTACTICAL_ERROR);
    }
    return 1;
  }
  return 0;
}

static inline int _NEXT(dx_adl_parser* parser) {
  return dx_adl_scanner_step(parser->scanner);
}

static inline int _EXPECT_AND_NEXT(dx_adl_parser* parser, dx_adl_word_type word_type, dx_string* word_text) {
  if (_EXPECT(parser, word_type, word_text)) {
    return 1;
  }
  if (_NEXT(parser)) {
    return 1;
  }
  return 0;
}

int dx_read_rgb_u8(dx_adl_parser* parser, DX_RGB_U8* color) {
  dx_string* text = NULL;
  dx_n8 r, g, b;

  // <name> "rgb"
  text = dx_string_create("rgb", sizeof("rgb") - 1);
  if (!text) {
    return 1;
  }
  if (_EXPECT_AND_NEXT(parser, dx_adl_word_type_name, text)) {
    DX_UNREFERENCE(text);
    text = NULL;
    return 1;
  }
  DX_UNREFERENCE(text);
  text = NULL;
  // <left parenthesis>
  if (_EXPECT_AND_NEXT(parser, dx_adl_word_type_left_parenthesis, NULL)) {
    return 1;
  }
  // <number>
  if (!_IS(parser, dx_adl_word_type_number, NULL)) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_n8(parser->scanner->text.elements, parser->scanner->text.size, &r)) {
    return 1;
  }
  if (_NEXT(parser)) {
    return 1;
  }
  // <comma>
  if (_EXPECT_AND_NEXT(parser, dx_adl_word_type_comma, NULL)) {
    return 1;
  }
  // <number>
  if (!_IS(parser, dx_adl_word_type_number, NULL)) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_n8(parser->scanner->text.elements, parser->scanner->text.size, &g)) {
    return 1;
  }
  if (_NEXT(parser)) {
    return 1;
  }
  // <comma>
  if (_EXPECT_AND_NEXT(parser, dx_adl_word_type_comma, NULL)) {
    return 1;
  }
  // <number>
  if (!_IS(parser, dx_adl_word_type_number, NULL)) {
    return 1;
  }
  if (dx_convert_utf8bytes_to_n8(parser->scanner->text.elements, parser->scanner->text.size, &b)) {
    return 1;
  }
  if (_NEXT(parser)) {
    return 1;
  }
  // <right parenthesis>
  if (_EXPECT_AND_NEXT(parser, dx_adl_word_type_right_parenthesis, NULL)) {
    return 1;
  }
  color->r = r;
  color->g = g;
  color->b = b;
  return 0;
}