#include "dx/ddl/parser.h"

#include <string.h>

DX_DEFINE_OBJECT_TYPE("dx.ddl.parser",
                      dx_ddl_parser,
                      dx_object)

/// @code
/// value := STRING | NUMBER | map | list
/// @endcode
static dx_ddl_node* dx_ddl_parser_on_value(dx_ddl_parser* p);

// @code
// map := LEFT_CURLY_BRACKET map_body RIGHT_CURLY_BRACKET
// map_body := e
//           | map_body_element map_body_rest
// map_body_rest := COMMA map_body_element map_body_rest
//                | COMMA
//                | e
// map_body_element := NAME COLON value
// @endcode
static dx_ddl_node* dx_ddl_parser_on_map(dx_ddl_parser* p);

static int dx_ddl_parser_on_map_0(dx_ddl_parser* p, dx_ddl_node* map_node);

// @code
// list := LEFT_SQUARE_BRACKET list_body RIGHT_SQUARE_BRACKET
// list_body := e
//            | list_element list_body_rest
// list_body_rest := COMMA list_element list_body_rest
//                 | COMMA
//                 | e
// @endcode
static dx_ddl_node* dx_ddl_parser_on_list(dx_ddl_parser* p);

static int dx_ddl_parser_on_list_0(dx_ddl_parser* p, dx_ddl_node* list_node);

static int dx_ddl_parser_next(dx_ddl_parser* self);

static dx_ddl_node* dx_ddl_parser_on_value(dx_ddl_parser* p) {
  switch (dx_ddl_parser_get_word_kind(p)) {
    case dx_ddl_word_kind_number: {
      dx_ddl_node* node = dx_ddl_node_create(dx_ddl_node_kind_number);
      if (!node) {
        return NULL;
      }
      dx_string* number_literal = dx_string_create(p->scanner->text.elements, p->scanner->text.size);
      if (!number_literal) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      if (dx_ddl_node_set_number(node, number_literal)) {
        DX_UNREFERENCE(number_literal);
        number_literal = NULL;
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      DX_UNREFERENCE(number_literal);
      number_literal = NULL;
      if (dx_ddl_parser_next(p)) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      return node;
    } break;
    case dx_ddl_word_kind_string: {
      dx_ddl_node* node = dx_ddl_node_create(dx_ddl_node_kind_string);
      if (!node) {
        return NULL;
      }
      dx_string* string_literal = dx_string_create(p->scanner->text.elements, p->scanner->text.size);
      if (!string_literal) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      if (dx_ddl_node_set_string(node, string_literal)) {
        DX_UNREFERENCE(string_literal);
        string_literal = NULL;
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      DX_UNREFERENCE(string_literal);
      string_literal = NULL;
      if (dx_ddl_parser_next(p)) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      return node;
    } break;
    case dx_ddl_word_kind_left_curly_bracket: {
      return dx_ddl_parser_on_map(p);
    } break;
    case dx_ddl_word_kind_left_square_bracket: {
      return dx_ddl_parser_on_list(p);
    } break;
    default: {
      dx_set_error(DX_SYNTACTICAL_ERROR);
      return NULL;
    } break;
  }
}

static int dx_ddl_parser_on_map_0(dx_ddl_parser* p, dx_ddl_node* map_node) {
  if (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_left_curly_bracket)) {
    return 1;
  }
  if (dx_ddl_parser_next(p)) {
    return 1;
  }
  while (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_right_curly_bracket)) {
    if (dx_get_error()) {
      return 1;
    }
    // name
    if (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_name)) {
      return 1;
    }
    dx_string* name = dx_string_create(p->scanner->text.elements, p->scanner->text.size);
    if (!name) {
      return 1;
    }
    if (dx_ddl_parser_next(p)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    // ':'
    if (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_colon)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    if (dx_ddl_parser_next(p)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    // value
    dx_ddl_node* value_node = dx_ddl_parser_on_value(p);
    if (!value_node) {
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    // put (name, value) into the map
    if (dx_ddl_node_map_set(map_node, name, value_node)) {
      DX_UNREFERENCE(value_node);
      value_node = NULL;
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    DX_UNREFERENCE(value_node);
    value_node = NULL;
    DX_UNREFERENCE(name);
    name = NULL;
    // if no comma follows, break the loop
    if (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_comma)) {
      if (dx_get_error()) {
        return 1;
      }
      break;
    }
    if (dx_ddl_parser_next(p)) {
      return 1;
    }
  }
  if (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_right_curly_bracket)) {
    dx_string* s;

    dx_log("syntactical error: received ", sizeof("syntactical error: received ") - 1);

    s = dx_ddl_word_kind_to_string(dx_ddl_parser_get_word_kind(p));
    if (!s) {
      return 1;
    }
    dx_log(dx_string_get_bytes(s), dx_string_get_number_of_bytes(s));
    DX_UNREFERENCE(s);
    s = NULL;

    dx_log(", expected ", sizeof(", expected ") - 1);
    s = dx_ddl_word_kind_to_string(dx_ddl_word_kind_right_curly_bracket);
    if (!s) {
      return 1;
    }
    dx_log(dx_string_get_bytes(s), dx_string_get_number_of_bytes(s));
    DX_UNREFERENCE(s);
    s = NULL;

    return 1;
  }
  if (dx_ddl_parser_next(p)) {
    return 1;
  }
  return 0;
}

static dx_ddl_node* dx_ddl_parser_on_map(dx_ddl_parser* p) {
  dx_ddl_node* node = dx_ddl_node_create(dx_ddl_node_kind_map);
  if (!node) {
    return NULL;
  }
  if (dx_ddl_parser_on_map_0(p, node)) {
    DX_UNREFERENCE(node);
    node = NULL;
    return NULL;
  }
  return node;
}

static int dx_ddl_parser_on_list_0(dx_ddl_parser* p, dx_ddl_node* list_node) {
  if (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_left_square_bracket)) {
    return 1;
  }
  if (dx_ddl_parser_next(p)) {
    return 1;
  }
  while (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_right_square_bracket)) {
    // value
    dx_ddl_node* value_node = dx_ddl_parser_on_value(p);
    if (!value_node) {
      return 1;
    }
    if (dx_ddl_node_list_append(list_node, value_node)) {
      DX_UNREFERENCE(value_node);
      value_node = NULL;
      return 1;
    }
    DX_UNREFERENCE(value_node);
    value_node = NULL;
    // if no comma follows, break the loop
    if (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_comma)) {
      if (dx_get_error()) {
        return 1;
      }
      break;
    }
    if (dx_ddl_parser_next(p)) {
      return 1;
    }
  }
  if (!dx_ddl_parser_is_word_kind(p, dx_ddl_word_kind_right_square_bracket)) {
    dx_string* s;

    dx_log("syntactical error: received ", sizeof("syntactical error: received ") - 1);

    s = dx_ddl_word_kind_to_string(dx_ddl_parser_get_word_kind(p));
    if (!s) {
      return 1;
    }
    dx_log(dx_string_get_bytes(s), dx_string_get_number_of_bytes(s));
    DX_UNREFERENCE(s);
    s = NULL;

    dx_log(", expected ", sizeof(", expected ") - 1);
    s = dx_ddl_word_kind_to_string(dx_ddl_word_kind_right_square_bracket);
    if (!s) {
      return 1;
    }
    dx_log(dx_string_get_bytes(s), dx_string_get_number_of_bytes(s));
    DX_UNREFERENCE(s);
    s = NULL;

    return 1;
  }
  if (dx_ddl_parser_next(p)) {
    return 1;
  }
  return 0;
}

static dx_ddl_node* dx_ddl_parser_on_list(dx_ddl_parser* p) {
  dx_ddl_node* node = dx_ddl_node_create(dx_ddl_node_kind_list);
  if (!node) {
    return NULL;
  }
  if (dx_ddl_parser_on_list_0(p, node)) {
    DX_UNREFERENCE(node);
    node = NULL;
    return NULL;
  }
  return node;
}

static int dx_ddl_parser_next(dx_ddl_parser* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_ddl_scanner_step(self->scanner);
}

int dx_ddl_parser_construct(dx_ddl_parser* self, dx_ddl_scanner* scanner, dx_adl_diagnostics* diagnostics) {
  dx_rti_type* _type = dx_ddl_parser_get_type();
  if (!_type) {
    return 1;
  }
  if (!self || !scanner || !diagnostics) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->scanner = scanner;
  DX_REFERENCE(self->scanner);
  self->diagnostics = diagnostics;
  DX_REFERENCE(self->diagnostics);
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_ddl_parser_destruct(dx_ddl_parser* self) {
  DX_UNREFERENCE(self->diagnostics);
  self->diagnostics = NULL;
  DX_UNREFERENCE(self->scanner);
  self->scanner = NULL;
}

dx_ddl_parser* dx_ddl_parser_create(dx_ddl_scanner* scanner, dx_adl_diagnostics* diagnostics) {
  dx_ddl_parser* self = DX_ADL_PARSER(dx_object_alloc(sizeof(dx_ddl_parser)));
  if (!self) {
    return NULL;
  }
  if (dx_ddl_parser_construct(self, scanner, diagnostics)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_ddl_parser_set(dx_ddl_parser* self, char const* p, dx_size l) {
  return dx_ddl_scanner_set(self->scanner, p, l);
}

bool dx_ddl_parser_is_word_kind(dx_ddl_parser const* self, dx_ddl_word_kind word_kind) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  bool result = (word_kind == dx_ddl_parser_get_word_kind(self));
  // Careful here. If an error occured, we must return false.
  return dx_get_error() ? false : result;
}

dx_ddl_word_kind dx_ddl_parser_get_word_kind(dx_ddl_parser const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  return self->scanner->kind;
}

dx_ddl_node* dx_ddl_parser_run(dx_ddl_parser* self) {
  // <start of input>
  if (!dx_ddl_parser_is_word_kind(self, dx_ddl_word_kind_start_of_input)) {
    return NULL;
  }
  if (dx_ddl_parser_next(self)) {
    return NULL;
  }
  dx_ddl_node* root_node = dx_ddl_parser_on_value(self);
  if (!root_node) {
    return NULL;
  }
  // <end of input>
  if (!dx_ddl_parser_is_word_kind(self, dx_ddl_word_kind_end_of_input)) {
    DX_UNREFERENCE(root_node);
    root_node = NULL;
  }
  return root_node;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_DDL_PARSER_WITH_TESTS) && DX_DDL_PARSER_WITH_TESTS

static dx_ddl_parser* dx_ddl_parser_test_create_parser() {
  dx_adl_diagnostics* diagnostics = dx_adl_diagnostics_create();
  if (!diagnostics) {
    return NULL;
  }
  dx_ddl_scanner* scanner = dx_ddl_scanner_create();
  if (!scanner) {
    DX_UNREFERENCE(diagnostics);
    diagnostics = NULL;
    return NULL;
  }
  dx_ddl_parser* parser = dx_ddl_parser_create(scanner, diagnostics);
  DX_UNREFERENCE(scanner);
  scanner = NULL;
  DX_UNREFERENCE(diagnostics);
  diagnostics = NULL;
  return parser;
}

static int dx_ddl_parser_test1() {
  dx_ddl_parser* parser = dx_ddl_parser_test_create_parser();
  if (parser) {
    DX_UNREFERENCE(parser);
    parser = NULL;
  }
  return dx_get_error();
}

static int dx_ddl_parser_test2() {
  static const char* input =
    "{\n"
    "  type : 'ColorRGBU8',\n"
    "  r : 255,\n"
    "  g : 0,\n"
    "  b : 0,\n"
    "}\n"
    ;
  dx_ddl_parser* parser = NULL;
  dx_ddl_node* root_node = NULL;
  
  parser = dx_ddl_parser_test_create_parser();
  if (!parser) {
    goto END;
  }
  if (dx_ddl_parser_set(parser, input, strlen(input))) {
    goto END;
  }
  root_node = dx_ddl_parser_run(parser);
  if (!root_node) {
    goto END;
  }
END:
  if (root_node) {
    DX_UNREFERENCE(root_node);
    root_node = NULL;
  }
  if (parser) {
    DX_UNREFERENCE(parser);
    parser = NULL;
  }
  return dx_get_error();
}

static int dx_ddl_parser_test3() {
  static const char* input =
    "{\n"
    "  type : 'CheckerboardBrush',"
    "  numberOfCheckers : {\n"
    "    horizontal: 16,\n"
    "    vertical: 16,\n"
    "  },\n"
    "  checkerSize: {\n"
    "    horizontal: 128,"
    "    vertical: 128,"
    "  },\n"
    "  checkerColors: {\n"
    "    first: 'malachite',"
    "    second: 'amber',"
    "  },\n"
    "}\n"
    ;
  dx_ddl_parser* parser = NULL;
  dx_ddl_node* root_node = NULL;

  parser = dx_ddl_parser_test_create_parser();
  if (!parser) {
    goto END;
  }
  if (dx_ddl_parser_set(parser, input, strlen(input))) {
    goto END;
  }
  root_node = dx_ddl_parser_run(parser);
  if (!root_node) {
    goto END;
  }
END:
  if (root_node) {
    DX_UNREFERENCE(root_node);
    root_node = NULL;
  }
  if (parser) {
    DX_UNREFERENCE(parser);
    parser = NULL;
  }
  return dx_get_error();
}

int dx_ddl_parser_tests() {
  if (dx_ddl_parser_test1()) {
    return 1;
  }
  if (dx_ddl_parser_test2()) {
    return 1;
  }
  if (dx_ddl_parser_test3()) {
    return 1;
  }
  return 0;
}

#endif // DX_DDL_PARSER_WITH_TESTS
