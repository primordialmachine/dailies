#include "dx/adl/syntactical.h"

#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_adl_word_construct(dx_adl_word* self, dx_adl_word_type type, dx_string* text) {
  if (!self || !text) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_adl_word_destruct;
  return 0;
}

void dx_adl_word_destruct(dx_adl_word* self) {
  DX_UNREFERENCE(self->text);
  self->text = NULL;
}

dx_adl_word* dx_adl_word_create(dx_adl_word_type type, dx_string* text) {
  dx_adl_word* self = DX_ADL_WORD(dx_object_alloc(sizeof(dx_adl_word)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_word_construct(self, type, text)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int dx_adl_scanner_save_and_next(dx_adl_scanner* self);

static bool dx_adl_scanner_is_digit(dx_adl_scanner* self);

static bool dx_adl_scanner_is_alphabetic(dx_adl_scanner* self);

static int dx_adl_scanner_on_single_quoted_string(dx_adl_scanner* self);

static int dx_adl_scanner_on_double_quoted_string(dx_adl_scanner* self);

static int dx_adl_scanner_on_name(dx_adl_scanner* self);

static int dx_adl_scanner_on_number(dx_adl_scanner* self);

static int dx_adl_scanner_skip_single_line_comment(dx_adl_scanner* self);

static int dx_adl_scanner_skip_multi_line_comment(dx_adl_scanner* self);

static int dx_adl_scanner_skip_nls_and_ws(dx_adl_scanner* self);

static int dx_adl_scanner_save_and_next(dx_adl_scanner* self) {
  if (self->current == self->end) {
    return 1;
  }
  if (dx_byte_array_append(&self->text, self->current, 1)) {
    return 1;
  }
  self->current++;
  return 0;
}

static bool dx_adl_scanner_is_digit(dx_adl_scanner* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  if (self->current == self->end) {
    return false;
  }
  return ('0' <= *self->current && *self->current <= '9');
}

static bool dx_adl_scanner_is_alphabetic(dx_adl_scanner* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  if (self->current == self->end) {
    return false;
  }
  return ('a' <= *self->current && *self->current <= 'z')
    || ('A' <= *self->current && *self->current <= 'Z');
}

static int dx_adl_scanner_on_single_quoted_string(dx_adl_scanner* self) {
  if (!(self->current != self->end && *self->current == '\'')) {
    dx_set_error(DX_LEXICAL_ERROR);
    return 1;
  }
  self->current++;
  bool lastWasSlash = false;
  while (true) {
    if (self->current == self->end) {
      // Unclosed string literal error.
      // Expected string contents or closing single quote.
      // Received end of input.
      dx_set_error(DX_LEXICAL_ERROR);
      return 1;
    } else if (*self->current == '\'') {
      if (lastWasSlash) {
        lastWasSlash = false;
        if (dx_adl_scanner_save_and_next(self)) {
          return 1;
        }
      } else {
        self->current++;
        self->type = dx_adl_word_type_string;
        return 0;
      }
    } else if (*self->current == '\\') {
      if (lastWasSlash) {
        lastWasSlash = false;
        if (dx_adl_scanner_save_and_next(self)) {
          return 1;
        }
      } else {
        lastWasSlash = true;
        self->current++;
      }
    } else {
      if (dx_adl_scanner_save_and_next(self)) {
        return 1;
      }
    }
  }
}

static int dx_adl_scanner_on_double_quoted_string(dx_adl_scanner* self) {
  if (!(self->current != self->end && *self->current == '\"')) {
    dx_set_error(DX_LEXICAL_ERROR);
    return 1;
  }
  self->current++;
  bool last_was_slash = false;
  while (true) {
    if (self->current == self->end) {
      // Unclosed string literal error.
      // Expected string contents or closing single quote.
      // Received end of input.
      dx_set_error(DX_LEXICAL_ERROR);
      return 1;
    } else if (*self->current == '\"') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_adl_scanner_save_and_next(self)) {
          return 1;
        }
      } else {
        self->current++;
        self->type = dx_adl_word_type_string;
        return 0;
      }
    } else if (*self->current == '\\') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_adl_scanner_save_and_next(self)) {
          return 1;
        }
      } else {
        last_was_slash = true;
        self->current++;
      }
    } else {
      if (dx_adl_scanner_save_and_next(self)) {
        return 1;
      }
    }
  }
}

static int dx_adl_scanner_on_name(dx_adl_scanner* self) {
  while (self->current != self->end && *self->current == '_') {
    if (dx_adl_scanner_save_and_next(self)) {
      return 1;
    }
  }
  if (!dx_adl_scanner_is_alphabetic(self)) {
    if (dx_get_error() == DX_NO_ERROR) {
      dx_set_error(DX_LEXICAL_ERROR);
    }
    return 1;
  }
  do {
    if (dx_adl_scanner_save_and_next(self)) {
      return 1;
    }
  } while (dx_adl_scanner_is_alphabetic(self) || (self->current != self->end && *self->current == '_') ||
    dx_adl_scanner_is_digit(self));
  if (dx_get_error()) {
    return 1;
  }
  self->type = dx_adl_word_type_name;
  return 0;
}

static int dx_adl_scanner_on_number(dx_adl_scanner* self) {
  // ('+'|'-')?
  if (self->current != self->end && (*self->current == '+' || *self->current == '-')) {
    if (dx_byte_array_append(&self->text, self->current, 1)) {
      return 1;
    }
    self->current++;
  }
  if (dx_adl_scanner_is_digit(self)) {
    // digit+ ('.' digit*)
    do {
      if (dx_byte_array_append(&self->text, self->current, 1)) {
        return 1;
      }
      self->current++;
    } while (dx_adl_scanner_is_digit(self));
    if (dx_get_error()) return 1;
    if (self->current != self->end && *self->current == '.') {
      if (dx_byte_array_append(&self->text, self->current, 1)) {
        return 1;
      }
      self->current++;
      while (dx_adl_scanner_is_digit(self)) {
        if (dx_byte_array_append(&self->text, self->current, 1)) {
          return 1;
        }
        self->current++;
      }
    }
  } else if (self->current != self->end && *self->current == '.') {
    // '.' digit+
    if (dx_byte_array_append(&self->text, self->current, 1)) {
      return 1;
    }
    if (!dx_adl_scanner_is_digit(self)) {
      if (!dx_get_error()) dx_set_error(DX_SYNTACTICAL_ERROR);
      return 1;
    }
    do {
      if (dx_byte_array_append(&self->text, self->current, 1)) {
        return 1;
      }
      self->current++;
    } while (dx_adl_scanner_is_digit(self));
    if (dx_get_error()) return 1;
  } else {
    dx_set_error(DX_LEXICAL_ERROR);
    return 1;
  }
  // exponent?
  // exponent = 'e'('+'|'-')? digit+
  if (self->current != self->end && *self->current == 'e') {
    if (dx_byte_array_append(&self->text, self->current, 1)) {
      return 1;
    }
    if (!dx_adl_scanner_is_digit(self)) {
      if (!dx_get_error()) dx_set_error(DX_SYNTACTICAL_ERROR);
      return 1;
    }
    do {
      if (dx_byte_array_append(&self->text, self->current, 1)) {
        return 1;
      }
    } while (dx_adl_scanner_is_digit(self));
    if (dx_get_error()) return 1;
  }
  self->type = dx_adl_word_type_number;
  return 0;
}

int dx_adl_scanner_construct(dx_adl_scanner* self) {
  static char const EMPTY[] = "";
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (dx_byte_array_construct(&self->text)) {
    return 1;
  }
  self->start = &EMPTY[0];
  self->end = self->start;
  self->current = self->start;
  self->type = dx_adl_word_type_start_of_input;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_adl_scanner_destruct;
  return 0;
}

static int dx_adl_scanner_skip_single_line_comment(dx_adl_scanner* self) {
  while (!(self->current == self->end)
    && !(self->current != self->end && *self->current == '\n')
    && !(self->current != self->end && *self->current == '\r')) {
    self->current++;
  }
  return 0;
}

static int dx_adl_scanner_skip_multi_line_comment(dx_adl_scanner* self) {
  bool last_was_star = false;
  while (true) {
    if (self->current == self->end) {
      dx_set_error(DX_LEXICAL_ERROR);
      return 1;
    } else if (*self->current == '*') {
      self->current++;
      if (self->current == self->end) {
        dx_set_error(DX_LEXICAL_ERROR);
        return 1;
      }
      if (*self->current == '/') {
        self->current++;
        return 0;
      } else {
        continue;
      }
    }
  }
  return 0;
}

static int dx_adl_scanner_skip_nls_and_ws(dx_adl_scanner* self) {
  bool stop;
  do {
    stop = true;
    // Skip whitespace.
    if ((*self->current == ' ' || *self->current == '\t') && self->current != self->end) {
      stop = false;
      do {
        self->current++;
      } while ((*self->current == ' ' || *self->current == '\t') && self->current != self->end);
    }
    // Skip newlines.
    while ((*self->current == '\n' || *self->current == '\r') && self->current != self->end) {
      stop = false;
      char old = *self->current;
      self->current++;
      if ((*self->current == '\n' || *self->current == '\r') && *self->current != old && self->current != self->end) {
        self->current++;
      }
    }
  } while (!stop);
  return 0;
}

void dx_adl_scanner_destruct(dx_adl_scanner* self) {
  dx_byte_array_destruct(&self->text);
}

dx_adl_scanner* dx_adl_scanner_create() {
  dx_adl_scanner* self = DX_ADL_SCANNER(dx_object_alloc(sizeof(dx_adl_scanner)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_scanner_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_adl_scanner_set(dx_adl_scanner* self, char const* p, size_t l) {
  self->start = p;
  self->end = p + l;
  self->current = p;
  self->type = dx_adl_word_type_start_of_input;
  dx_byte_array_clear(&self->text);
  return 0;
}

int dx_adl_scanner_step(dx_adl_scanner* self) {
START:
  // If we reached the end of the input, we return immediatly.
  if (self->type == dx_adl_word_type_end_of_input) {
    return 0;
  }
  if (dx_adl_scanner_skip_nls_and_ws(self)) {
    return 1;
  }
  // We have reached the end of the input.
  if (self->current == self->end) {
    dx_byte_array_clear(&self->text);
    self->type = dx_adl_word_type_end_of_input;
    return 0;
  }
  switch (*self->current) {
    case '/': {
      self->current++;
      if (self->current == self->end) {
        if (dx_get_error() == DX_NO_ERROR) {
          dx_set_error(DX_LEXICAL_ERROR);
        }
        return 1;
      }
      if (*self->current == '/') {
        self->current++;
        if (dx_adl_scanner_skip_single_line_comment(self)) {
          return 1;
        }
        goto START;
      } else if (*self->current == '*') {
        self->current++;
        if (dx_adl_scanner_skip_multi_line_comment(self)) {
          return 1;
        }
        goto START;
      } else {
        if (dx_get_error() == DX_NO_ERROR) {
          dx_set_error(DX_LEXICAL_ERROR);
        }
        return 1;
      }
    } break;
    case ',': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, ",", sizeof(",") - 1)) {
        return 1;
      }
      self->type = dx_adl_word_type_comma;
      self->current++;
      return 0;
    } break;
    case '(': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "(", sizeof("(") - 1)) {
        return 1;
      }
      self->type = dx_adl_word_type_left_parenthesis;
      self->current++;
      return 0;
    } break;
    case ')': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, ")", sizeof(")") - 1)) {
        return 1;
      }
      self->type = dx_adl_word_type_right_parenthesis;
      self->current++;
      return 0;
    } break;

    case '[': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "[", sizeof("[") - 1)) {
        return 1;
      }
      self->type = dx_adl_word_type_left_square_bracket;
      self->current++;
      return 0;
    } break;
    case ']': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "]", sizeof("]") - 1)) {
        return 1;
      }
      self->type = dx_adl_word_type_right_square_bracket;
      self->current++;
      return 0;
    } break;

    case '{': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "{", sizeof("{") - 1)) {
        return 1;
      }
      self->type = dx_adl_word_type_left_curly_bracket;
      self->current++;
      return 0;
    } break;
    case '}': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "}", sizeof("}") - 1)) {
        return 1;
      }
      self->type = dx_adl_word_type_right_curly_bracket;
      self->current++;
      return 0;
    } break;

    case ':': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, ":", sizeof(":") - 1)) {
        return 1;
      }
      self->type = dx_adl_word_type_colon;
      self->current++;
      return 0;
    } break;

    case '\'': {
      dx_byte_array_clear(&self->text);
      return dx_adl_scanner_on_single_quoted_string(self);
    } break;
    case '"': {
      dx_byte_array_clear(&self->text);
      return dx_adl_scanner_on_double_quoted_string(self);
    } break;
    case '0': case '4': case '8':
    case '1': case '5': case '9':
    case '2': case '6':
    case '3': case '7':
    case '+':
    case '-':
    case '.': {
      dx_byte_array_clear(&self->text);
      return dx_adl_scanner_on_number(self);
    } break;
    default: {
      dx_byte_array_clear(&self->text);
      return dx_adl_scanner_on_name(self);
    } break;
  };
}

char const* dx_adl_scanner_get_word_text_bytes(dx_adl_scanner const* self) {
  return self->text.elements;
}

size_t dx_adl_scanner_get_word_text_number_of_bytes(dx_adl_scanner const* self) {
  return self->text.size;
}

dx_adl_word_type dx_adl_scanner_get_word_type(dx_adl_scanner const* self) {
  return self->type;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void on_added(dx_object** a);

static void on_removed(dx_object** a);

static size_t on_hash_key(dx_object** a);

static bool on_compare_keys(dx_object** a, dx_object** b);

static void on_added(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_REFERENCE(*a);
}

static void on_removed(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_UNREFERENCE(*a);
}

static size_t on_hash_key(dx_object** a) {
  return dx_string_get_hash_value(DX_STRING(*a));
}

static bool on_compare_keys(dx_object** a, dx_object** b) {
  return dx_string_is_equal_to(DX_STRING(*a), DX_STRING(*b));
}

int dx_adl_node_construct(dx_adl_node* self, dx_adl_node_type type) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->type = type;
  switch (self->type) {
    case dx_adl_node_type_error: {
      return 0;
    } break;
    case dx_adl_node_type_list: {
      if (dx_pointer_array_initialize(&self->list, 0, (void(*)(void**)) & on_added, (void(*)(void**)) &on_removed)) {
        return 1;
      }
    } break;
    case dx_adl_node_type_map: {
      static DX_POINTER_HASHMAP_CONFIGURATION const configuration = {
        .compare_keys_callback = (bool(*)(void**,void**)) &on_compare_keys,
        .hash_key_callback = (size_t(*)(void**)) & on_hash_key,
        .key_added_callback = (void(*)(void**)) &on_added,
        .key_removed_callback = (void(*)(void**)) &on_removed,
        .value_added_callback = (void(*)(void**)) &on_added,
        .value_removed_callback = (void(*)(void**)) &on_removed,
      };
      if (dx_pointer_hashmap_initialize(&self->map, &configuration)) {
        return 1;
      }
    } break;
    case dx_adl_node_type_number: {
      self->number = dx_string_create("0", sizeof("0") - 1);
      if (!self->number) {
        return 1;
      }
    } break;
    case dx_adl_node_type_string: {
      self->string = dx_string_create("", sizeof("") - 1);
      if (!self->string) {
        return 1;
      }
    } break;
    default: {
      dx_set_error(DX_INVALID_ARGUMENT);
      return 1;
    } break;
  };
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_adl_node_destruct;
  return 0;
}

void dx_adl_node_destruct(dx_adl_node* self) {
  DX_DEBUG_ASSERT(NULL != self);
  switch (self->type) {
    case dx_adl_node_type_error: {
    } break;
    case dx_adl_node_type_list: {
      dx_pointer_array_uninitialize(&self->list);
    } break;
    case dx_adl_node_type_map: {
      dx_pointer_hashmap_uninitialize(&self->map);
    } break;
    case dx_adl_node_type_number: {
      DX_DEBUG_ASSERT(NULL != self->number);
      DX_DEBUG_ASSERT(NULL != self->string);
      DX_UNREFERENCE(self->number);
      self->number = NULL;
    } break;
    case dx_adl_node_type_string: {
      DX_DEBUG_ASSERT(NULL != self->string);
      DX_UNREFERENCE(self->string);
      self->string = NULL;
    } break;
  };
}

dx_adl_node* dx_adl_node_create(dx_adl_node_type type) {
  dx_adl_node* self = DX_ADL_NODE(dx_object_alloc(sizeof(dx_adl_node)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_node_construct(self, type)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

dx_adl_node_type dx_adl_node_get_type(dx_adl_node const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return self->type;
}

int dx_adl_node_map_set(dx_adl_node* self, dx_string* name, dx_adl_node* value) {
  if (!self || !name || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->type != dx_adl_node_type_map) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return dx_pointer_hashmap_set(&self->map, name, value);
}

dx_adl_node* dx_adl_node_map_get(dx_adl_node const* self, dx_string* name) {
  if (!self || !name) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->type != dx_adl_node_type_map) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  return dx_pointer_hashmap_get(&self->map, name);
}

int dx_adl_node_list_append(dx_adl_node* self, dx_adl_node* value) {
  if (!self || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->type != dx_adl_node_type_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return dx_pointer_array_append(&self->list, (void*)value);
}

int dx_adl_node_list_prepend(dx_adl_node* self, dx_adl_node* value) {
  if (!self || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->type != dx_adl_node_type_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return dx_pointer_array_prepend(&self->list, (void*)value);
}

int dx_adl_node_list_insert(dx_adl_node* self, dx_adl_node* value, dx_size index) {
  if (!self || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->type != dx_adl_node_type_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return dx_pointer_array_insert(&self->list, (void*)value, index);
}

dx_adl_node* dx_adl_node_list_get(dx_adl_node* self, dx_size index) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->type != dx_adl_node_type_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  return dx_pointer_array_get_at(&self->list, index);
}

dx_size dx_adl_node_list_get_size(dx_adl_node* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  if (self->type != dx_adl_node_type_list) {
    dx_set_error(DX_INVALID_OPERATION);
    return 0;
  }
  return dx_pointer_array_get_size(&self->list);
}

dx_string* dx_adl_node_get_string(dx_adl_node const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->type != dx_adl_node_type_string) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  return self->string;
}

int dx_adl_node_set_string(dx_adl_node* self, dx_string* string) {
  if (!self || !string) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->type != dx_adl_node_type_string) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  DX_REFERENCE(string);
  DX_UNREFERENCE(self->string);
  self->string = string;
  return 0;
}

dx_string* dx_adl_node_get_number(dx_adl_node const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->type != dx_adl_node_type_number) {
    dx_set_error(DX_INVALID_OPERATION);
    return NULL;
  }
  return self->number;
}

int dx_adl_node_set_number(dx_adl_node* self, dx_string* number) {
  if (!self || !number) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (self->type != dx_adl_node_type_number) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  DX_REFERENCE(number);
  DX_UNREFERENCE(self->number);
  self->number = number;
  return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @code
/// value := STRING | NUMBER | map | list
/// @endcode
static dx_adl_node* dx_adl_parser_on_value(dx_adl_parser* p);

// @code
// map := LEFT_CURLY_BRACKET map_body RIGHT_CURLY_BRACKET
// map_body := e
//           | map_body_element map_body_rest
// map_body_rest := COMMA map_body_element map_body_rest
//                | COMMA
//                | e
// map_body_element := NAME COLON value
// @endcode
static dx_adl_node* dx_adl_parser_on_map(dx_adl_parser* p);

static int dx_adl_parser_on_map_0(dx_adl_parser* p, dx_adl_node* map_node);

// @code
// list := LEFT_SQUARE_BRACKET list_body RIGHT_SQUARE_BRACKET
// list_body := e
//            | list_element list_body_rest
// list_body_rest := COMMA list_element list_body_rest
//                 | COMMA
//                 | e
// @endcode
static dx_adl_node* dx_adl_parser_on_list(dx_adl_parser* p);

static int dx_adl_parser_on_list_0(dx_adl_parser* p, dx_adl_node* list_node);

static int dx_adl_parser_next(dx_adl_parser* self);

static dx_adl_node* dx_adl_parser_on_value(dx_adl_parser* p) {
  switch (dx_adl_parser_get_word_type(p)) {
    case dx_adl_word_type_number: {
      dx_adl_node* node = dx_adl_node_create(dx_adl_node_type_number);
      if (!node) {
        return NULL;
      }
      dx_string* number_literal = dx_string_create(p->scanner->text.elements, p->scanner->text.size);
      if (!number_literal) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      if (dx_adl_node_set_number(node, number_literal)) {
        DX_UNREFERENCE(number_literal);
        number_literal = NULL;
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      DX_UNREFERENCE(number_literal);
      number_literal = NULL;
      if (dx_adl_parser_next(p)) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      return node;
    } break;
    case dx_adl_word_type_string: {
      dx_adl_node* node = dx_adl_node_create(dx_adl_node_type_string);
      if (!node) {
        return NULL;
      }
      dx_string* string_literal = dx_string_create(p->scanner->text.elements, p->scanner->text.size);
      if (!string_literal) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      if (dx_adl_node_set_string(node, string_literal)) {
        DX_UNREFERENCE(string_literal);
        string_literal = NULL;
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      DX_UNREFERENCE(string_literal);
      string_literal = NULL;
      if (dx_adl_parser_next(p)) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      return node;
    } break;
    case dx_adl_word_type_left_curly_bracket: {
      return dx_adl_parser_on_map(p);
    } break;
    case dx_adl_word_type_left_square_bracket: {
      return dx_adl_parser_on_list(p);
    } break;
    default: {
      dx_set_error(DX_SYNTACTICAL_ERROR);
      return NULL;
    } break;
  }
}

static int dx_adl_parser_on_map_0(dx_adl_parser* p, dx_adl_node* map_node) {
  if (!dx_adl_parser_is_word_type(p, dx_adl_word_type_left_curly_bracket)) {
    return 1;
  }
  if (dx_adl_parser_next(p)) {
    return 1;
  }
  while (!dx_adl_parser_is_word_type(p, dx_adl_word_type_right_curly_bracket)) {
    if (dx_get_error()) {
      return 1;
    }
    // name
    if (!dx_adl_parser_is_word_type(p, dx_adl_word_type_name)) {
      return 1;
    }
    dx_string* name = dx_string_create(p->scanner->text.elements, p->scanner->text.size);
    if (!name) {
      return 1;
    }
    if (dx_adl_parser_next(p)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    // ':'
    if (!dx_adl_parser_is_word_type(p, dx_adl_word_type_colon)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    if (dx_adl_parser_next(p)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    // value
    dx_adl_node* value_node = dx_adl_parser_on_value(p);
    if (!value_node) {
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    // put (name, value) into the map
    if (dx_adl_node_map_set(map_node, name, value_node)) {
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
    if (!dx_adl_parser_is_word_type(p, dx_adl_word_type_comma)) {
      if (dx_get_error()) {
        return 1;
      }
      break;
    }
    if (dx_adl_parser_next(p)) {
      return 1;
    }
  }
  if (!dx_adl_parser_is_word_type(p, dx_adl_word_type_right_curly_bracket)) {
    return 1;
  }
  if (dx_adl_parser_next(p)) {
    return 1;
  }
  return 0;
}

static dx_adl_node* dx_adl_parser_on_map(dx_adl_parser* p) {
  dx_adl_node* node = dx_adl_node_create(dx_adl_node_type_map);
  if (!node) {
    return NULL;
  }
  if (dx_adl_parser_on_map_0(p, node)) {
    DX_UNREFERENCE(node);
    node = NULL;
    return NULL;
  }
  return node;
}

static int dx_adl_parser_on_list_0(dx_adl_parser* p, dx_adl_node* list_node) {
  if (!dx_adl_parser_is_word_type(p, dx_adl_word_type_left_square_bracket)) {
    return 1;
  }
  if (dx_adl_parser_next(p)) {
    return 1;
  }
  while (!dx_adl_parser_is_word_type(p, dx_adl_word_type_right_square_bracket)) {
    // value
    dx_adl_node* value_node = dx_adl_parser_on_value(p);
    if (!value_node) {
      return 1;
    }
    if (dx_adl_node_list_append(list_node, value_node)) {
      DX_UNREFERENCE(value_node);
      value_node = NULL;
      return 1;
    }
    DX_UNREFERENCE(value_node);
    value_node = NULL;
    // if no comma follows, break the loop
    if (!dx_adl_parser_is_word_type(p, dx_adl_word_type_comma)) {
      if (dx_get_error()) {
        return 1;
      }
      break;
    }
    if (dx_adl_parser_next(p)) {
      return 1;
    }
  }
  if (!dx_adl_parser_is_word_type(p, dx_adl_word_type_right_square_bracket)) {
    return 1;
  }
  if (dx_adl_parser_next(p)) {
    return 1;
  }
  return 0;
}

static dx_adl_node* dx_adl_parser_on_list(dx_adl_parser* p) {
  dx_adl_node* node = dx_adl_node_create(dx_adl_node_type_list);
  if (!node) {
    return NULL;
  }
  if (dx_adl_parser_on_list_0(p, node)) {
    DX_UNREFERENCE(node);
    node = NULL;
    return NULL;
  }
  return node;
}

static int dx_adl_parser_next(dx_adl_parser* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  return dx_adl_scanner_step(self->scanner);
}

int dx_adl_parser_construct(dx_adl_parser* self, dx_adl_scanner* scanner, dx_adl_diagnostics* diagnostics) {
  if (!self || !scanner || !diagnostics) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->scanner = scanner;
  DX_REFERENCE(self->scanner);
  self->diagnostics = diagnostics;
  DX_REFERENCE(self->diagnostics);
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_adl_parser_destruct;
  return 0;
}

void dx_adl_parser_destruct(dx_adl_parser* self) {
  DX_UNREFERENCE(self->diagnostics);
  self->diagnostics = NULL;
  DX_UNREFERENCE(self->scanner);
  self->scanner = NULL;
}

dx_adl_parser* dx_adl_parser_create(dx_adl_scanner* scanner, dx_adl_diagnostics* diagnostics) {
  dx_adl_parser* self = DX_ADL_PARSER(dx_object_alloc(sizeof(dx_adl_parser)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_parser_construct(self, scanner, diagnostics)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_adl_parser_set(dx_adl_parser* self, char const* p, size_t l) {
  return dx_adl_scanner_set(self->scanner, p, l);
}

bool dx_adl_parser_is_word_type(dx_adl_parser const* self, dx_adl_word_type word_type) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  bool result = (word_type == dx_adl_parser_get_word_type(self));
  // Careful here. If an error occured, we must return false.
  return dx_get_error() ? false : result;
}

dx_adl_word_type dx_adl_parser_get_word_type(dx_adl_parser const* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  return self->scanner->type;
}

dx_adl_node* dx_adl_parser_run(dx_adl_parser* self) {
  // <start of input>
  if (!dx_adl_parser_is_word_type(self, dx_adl_word_type_start_of_input)) {
    return NULL;
  }
  if (dx_adl_parser_next(self)) {
    return NULL;
  }
  dx_adl_node* root_node = dx_adl_parser_on_value(self);
  if (!root_node) {
    return NULL;
  }
  // <end of input>
  if (!dx_adl_parser_is_word_type(self, dx_adl_word_type_end_of_input)) {
    DX_UNREFERENCE(root_node);
    root_node = NULL;
  }
  return root_node;
}

#if defined(DX_ADL_PARSER_WITH_TESTS) && DX_ADL_PARSER_WITH_TESTS

static dx_adl_parser* dx_adl_parser_test_create_parser() {
  dx_adl_diagnostics* diagnostics = dx_adl_diagnostics_create();
  if (!diagnostics) {
    return NULL;
  }
  dx_adl_scanner* scanner = dx_adl_scanner_create();
  if (!scanner) {
    DX_UNREFERENCE(diagnostics);
    diagnostics = NULL;
    return NULL;
  }
  dx_adl_parser* parser = dx_adl_parser_create(scanner, diagnostics);
  DX_UNREFERENCE(scanner);
  scanner = NULL;
  DX_UNREFERENCE(diagnostics);
  diagnostics = NULL;
  return parser;
}

static int dx_adl_parser_test1() {
  dx_adl_parser* parser = dx_adl_parser_test_create_parser();
  if (parser) {
    DX_UNREFERENCE(parser);
    parser = NULL;
  }
  return dx_get_error();
}

static int dx_adl_parser_test2() {
  static const char* input =
    "{\n"
    "  type : 'ColorRGBU8',\n"
    "  r : 255,\n"
    "  g : 0,\n"
    "  b : 0,\n"
    "}\n"
    ;
  dx_adl_parser* parser = NULL;
  dx_adl_node* root_node = NULL;
  
  parser = dx_adl_parser_test_create_parser();
  if (!parser) {
    goto END;
  }
  if (dx_adl_parser_set(parser, input, strlen(input))) {
    goto END;
  }
  root_node = dx_adl_parser_run(parser);
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

static int dx_adl_parser_test3() {
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
  dx_adl_parser* parser = NULL;
  dx_adl_node* root_node = NULL;

  parser = dx_adl_parser_test_create_parser();
  if (!parser) {
    goto END;
  }
  if (dx_adl_parser_set(parser, input, strlen(input))) {
    goto END;
  }
  root_node = dx_adl_parser_run(parser);
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

int dx_adl_parser_tests() {
  if (dx_adl_parser_test1()) {
    return 1;
  }
  if (dx_adl_parser_test2()) {
    return 1;
  }
  if (dx_adl_parser_test3()) {
    return 1;
  }
  return 0;
}

#endif // DX_ADL_PARSER_WITH_TESTS
