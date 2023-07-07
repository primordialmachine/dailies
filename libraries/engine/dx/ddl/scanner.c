#include "dx/ddl/scanner.h"

#include <string.h>

static int dx_ddl_scanner_save_and_next(dx_ddl_scanner* self);

static bool dx_ddl_scanner_is_digit(dx_ddl_scanner* self);

static bool dx_ddl_scanner_is_alphabetic(dx_ddl_scanner* self);

static int dx_ddl_scanner_on_single_quoted_string(dx_ddl_scanner* self);

static int dx_ddl_scanner_on_double_quoted_string(dx_ddl_scanner* self);

static int dx_ddl_scanner_on_name(dx_ddl_scanner* self);

static int dx_ddl_scanner_on_number(dx_ddl_scanner* self);

static int dx_ddl_scanner_skip_single_line_comment(dx_ddl_scanner* self);

static int dx_ddl_scanner_skip_multi_line_comment(dx_ddl_scanner* self);

static int dx_ddl_scanner_skip_nls_and_ws(dx_ddl_scanner* self);

DX_DEFINE_OBJECT_TYPE("dx.ddl.scanner",
                      dx_ddl_scanner,
                      dx_object)

static int dx_ddl_scanner_save_and_next(dx_ddl_scanner* self) {
  if (self->current == self->end) {
    return 1;
  }
  if (dx_byte_array_append(&self->text, self->current, 1)) {
    return 1;
  }
  self->current++;
  return 0;
}

static bool dx_ddl_scanner_is_digit(dx_ddl_scanner* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return false;
  }
  if (self->current == self->end) {
    return false;
  }
  return ('0' <= *self->current && *self->current <= '9');
}

static bool dx_ddl_scanner_is_alphabetic(dx_ddl_scanner* self) {
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

static int dx_ddl_scanner_on_single_quoted_string(dx_ddl_scanner* self) {
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
        if (dx_ddl_scanner_save_and_next(self)) {
          return 1;
        }
      } else {
        self->current++;
        self->kind = dx_ddl_word_kind_string;
        return 0;
      }
    } else if (*self->current == '\\') {
      if (lastWasSlash) {
        lastWasSlash = false;
        if (dx_ddl_scanner_save_and_next(self)) {
          return 1;
        }
      } else {
        lastWasSlash = true;
        self->current++;
      }
    } else {
      if (dx_ddl_scanner_save_and_next(self)) {
        return 1;
      }
    }
  }
}

static int dx_ddl_scanner_on_double_quoted_string(dx_ddl_scanner* self) {
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
        if (dx_ddl_scanner_save_and_next(self)) {
          return 1;
        }
      } else {
        self->current++;
        self->kind = dx_ddl_word_kind_string;
        return 0;
      }
    } else if (*self->current == '\\') {
      if (last_was_slash) {
        last_was_slash = false;
        if (dx_ddl_scanner_save_and_next(self)) {
          return 1;
        }
      } else {
        last_was_slash = true;
        self->current++;
      }
    } else {
      if (dx_ddl_scanner_save_and_next(self)) {
        return 1;
      }
    }
  }
}

static int dx_ddl_scanner_on_name(dx_ddl_scanner* self) {
  while (self->current != self->end && *self->current == '_') {
    if (dx_ddl_scanner_save_and_next(self)) {
      return 1;
    }
  }
  if (!dx_ddl_scanner_is_alphabetic(self)) {
    if (dx_get_error() == DX_NO_ERROR) {
      dx_set_error(DX_LEXICAL_ERROR);
    }
    return 1;
  }
  do {
    if (dx_ddl_scanner_save_and_next(self)) {
      return 1;
    }
  } while (dx_ddl_scanner_is_alphabetic(self) || (self->current != self->end && *self->current == '_') ||
    dx_ddl_scanner_is_digit(self));
  if (dx_get_error()) {
    return 1;
  }
  self->kind = dx_ddl_word_kind_name;
  return 0;
}

static int dx_ddl_scanner_on_number(dx_ddl_scanner* self) {
  // ('+'|'-')?
  if (self->current != self->end && (*self->current == '+' || *self->current == '-')) {
    if (dx_byte_array_append(&self->text, self->current, 1)) {
      return 1;
    }
    self->current++;
  }
  if (dx_ddl_scanner_is_digit(self)) {
    // digit+ ('.' digit*)
    do {
      if (dx_byte_array_append(&self->text, self->current, 1)) {
        return 1;
      }
      self->current++;
    } while (dx_ddl_scanner_is_digit(self));
    if (dx_get_error()) return 1;
    if (self->current != self->end && *self->current == '.') {
      if (dx_byte_array_append(&self->text, self->current, 1)) {
        return 1;
      }
      self->current++;
      while (dx_ddl_scanner_is_digit(self)) {
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
    if (!dx_ddl_scanner_is_digit(self)) {
      if (!dx_get_error()) dx_set_error(DX_SYNTACTICAL_ERROR);
      return 1;
    }
    do {
      if (dx_byte_array_append(&self->text, self->current, 1)) {
        return 1;
      }
      self->current++;
    } while (dx_ddl_scanner_is_digit(self));
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
    if (!dx_ddl_scanner_is_digit(self)) {
      if (!dx_get_error()) dx_set_error(DX_SYNTACTICAL_ERROR);
      return 1;
    }
    do {
      if (dx_byte_array_append(&self->text, self->current, 1)) {
        return 1;
      }
    } while (dx_ddl_scanner_is_digit(self));
    if (dx_get_error()) return 1;
  }
  self->kind = dx_ddl_word_kind_number;
  return 0;
}

int dx_ddl_scanner_construct(dx_ddl_scanner* self) {
  dx_rti_type* _type = dx_ddl_scanner_get_type();
  if (!_type) {
    return 1;
  }
  static char const EMPTY[] = "";
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (dx_byte_array_initialize(&self->text)) {
    return 1;
  }
  self->start = &EMPTY[0];
  self->end = self->start;
  self->current = self->start;
  self->kind = dx_ddl_word_kind_start_of_input;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static int dx_ddl_scanner_skip_single_line_comment(dx_ddl_scanner* self) {
  while (!(self->current == self->end)
      && !(self->current != self->end && *self->current == '\n')
      && !(self->current != self->end && *self->current == '\r')) {
    self->current++;
  }
  return 0;
}

static int dx_ddl_scanner_skip_multi_line_comment(dx_ddl_scanner* self) {
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

static int dx_ddl_scanner_skip_nls_and_ws(dx_ddl_scanner* self) {
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

static void dx_ddl_scanner_destruct(dx_ddl_scanner* self) {
  dx_byte_array_uninitialize(&self->text);
}

dx_ddl_scanner* dx_ddl_scanner_create() {
  dx_ddl_scanner* self = DX_ADL_SCANNER(dx_object_alloc(sizeof(dx_ddl_scanner)));
  if (!self) {
    return NULL;
  }
  if (dx_ddl_scanner_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_ddl_scanner_set(dx_ddl_scanner* self, char const* p, dx_size l) {
  self->start = p;
  self->end = p + l;
  self->current = p;
  self->kind = dx_ddl_word_kind_start_of_input;
  dx_byte_array_clear(&self->text);
  return 0;
}

int dx_ddl_scanner_step(dx_ddl_scanner* self) {
START:
  // If we reached the end of the input, we return immediatly.
  if (self->kind == dx_ddl_word_kind_end_of_input) {
    return 0;
  }
  if (dx_ddl_scanner_skip_nls_and_ws(self)) {
    return 1;
  }
  // We have reached the end of the input.
  if (self->current == self->end) {
    dx_byte_array_clear(&self->text);
    self->kind = dx_ddl_word_kind_end_of_input;
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
        if (dx_ddl_scanner_skip_single_line_comment(self)) {
          return 1;
        }
        goto START;
      } else if (*self->current == '*') {
        self->current++;
        if (dx_ddl_scanner_skip_multi_line_comment(self)) {
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
      self->kind = dx_ddl_word_kind_comma;
      self->current++;
      return 0;
    } break;
    case '(': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "(", sizeof("(") - 1)) {
        return 1;
      }
      self->kind = dx_ddl_word_kind_left_parenthesis;
      self->current++;
      return 0;
    } break;
    case ')': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, ")", sizeof(")") - 1)) {
        return 1;
      }
      self->kind = dx_ddl_word_kind_right_parenthesis;
      self->current++;
      return 0;
    } break;

    case '[': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "[", sizeof("[") - 1)) {
        return 1;
      }
      self->kind = dx_ddl_word_kind_left_square_bracket;
      self->current++;
      return 0;
    } break;
    case ']': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "]", sizeof("]") - 1)) {
        return 1;
      }
      self->kind = dx_ddl_word_kind_right_square_bracket;
      self->current++;
      return 0;
    } break;

    case '{': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "{", sizeof("{") - 1)) {
        return 1;
      }
      self->kind = dx_ddl_word_kind_left_curly_bracket;
      self->current++;
      return 0;
    } break;
    case '}': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, "}", sizeof("}") - 1)) {
        return 1;
      }
      self->kind = dx_ddl_word_kind_right_curly_bracket;
      self->current++;
      return 0;
    } break;

    case ':': {
      dx_byte_array_clear(&self->text);
      if (dx_byte_array_append(&self->text, ":", sizeof(":") - 1)) {
        return 1;
      }
      self->kind = dx_ddl_word_kind_colon;
      self->current++;
      return 0;
    } break;

    case '\'': {
      dx_byte_array_clear(&self->text);
      return dx_ddl_scanner_on_single_quoted_string(self);
    } break;
    case '"': {
      dx_byte_array_clear(&self->text);
      return dx_ddl_scanner_on_double_quoted_string(self);
    } break;
    case '0': case '4': case '8':
    case '1': case '5': case '9':
    case '2': case '6':
    case '3': case '7':
    case '+':
    case '-':
    case '.': {
      dx_byte_array_clear(&self->text);
      return dx_ddl_scanner_on_number(self);
    } break;
    default: {
      dx_byte_array_clear(&self->text);
      return dx_ddl_scanner_on_name(self);
    } break;
  };
}

char const* dx_ddl_scanner_get_word_text_bytes(dx_ddl_scanner const* self) {
  return self->text.elements;
}

dx_size dx_ddl_scanner_get_word_text_number_of_bytes(dx_ddl_scanner const* self) {
  return self->text.size;
}

dx_ddl_word_kind dx_ddl_scanner_get_word_kind(dx_ddl_scanner const* self) {
  return self->kind;
}
