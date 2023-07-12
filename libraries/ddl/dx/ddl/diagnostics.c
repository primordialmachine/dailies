#include "dx/ddl/diagnostics.h"

DX_DEFINE_OBJECT_TYPE("dx.ddl.diagnostics",
                      dx_ddl_diagnostics,
                      dx_object)

int dx_ddl_diagnostics_construct(dx_ddl_diagnostics* self) {
  dx_rti_type* _type = dx_ddl_diagnostics_get_type();
  if (!_type) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_ddl_diagnostics_destruct(dx_ddl_diagnostics* self)
{/*Intentionally empty.*/}

dx_ddl_diagnostics* dx_ddl_diagnostics_create() {
  dx_ddl_diagnostics* self = DX_DDL_DIAGNOSTICS(dx_object_alloc(sizeof(dx_ddl_diagnostics)));
  if (!self) {
    return NULL;
  }
  if (dx_ddl_diagnostics_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_ddl_diagnostics_on_lexical(dx_ddl_diagnostics* self) {
  dx_string* format = dx_string_create("lexical error:\n", sizeof("lexical error:\n"));
  if (!format) {
    return 1;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  return 0;
}

int dx_ddl_diagnostics_on_unclosed_string_literal(dx_ddl_diagnostics* self) {
  dx_string* format = dx_string_create("lexical error: unclosed string literal\n", sizeof("lexical error: unclosed string literal\n") - 1);
  if (!format) {
    return 1;
  }
  dx_log(format->bytes, format->number_of_bytes);
  DX_UNREFERENCE(format);
  format = NULL;
  return 0;
}

int dx_ddl_diagnostics_on_invalid_number_literal(dx_ddl_diagnostics* self) {
  dx_string* format = dx_string_create("lexical error: invalid number literal\n", sizeof("lexical error: invalid number literal\n") - 1);
  if (!format) {
    return 1;
  }
  dx_log(format->bytes, format->number_of_bytes);
  DX_UNREFERENCE(format);
  format = NULL;
  return 0;
}

int dx_ddl_diagnostics_on_unclosed_multi_line_comment(dx_ddl_diagnostics* self) {
  dx_string* format = dx_string_create("lexical error: unclosed multi-line comment\n", sizeof("lexical error: unclosed multi-line comment\n") - 1);
  if (!format) {
    return 1;
  }
  dx_log(format->bytes, format->number_of_bytes);
  DX_UNREFERENCE(format);
  format = NULL;
  return 0;
}

int dx_ddl_diagnostics_on_unexpected_symbol(dx_ddl_diagnostics* self) {
  dx_string* format = dx_string_create("lexical error: unexpected symbol\n", sizeof("lexical error: unexpected symbol\n") - 1);
  if (!format) {
    return 1;
  }
  dx_log(format->bytes, format->number_of_bytes);
  DX_UNREFERENCE(format);
  format = NULL;
  return 0;
}

