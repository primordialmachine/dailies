#include "dx/ddl/word.h"

DX_DEFINE_OBJECT_TYPE("dx.ddl.word",
                      dx_ddl_word,
                      dx_object)

int dx_ddl_word_construct(dx_ddl_word* self, dx_ddl_word_kind kind, dx_string* text) {
  dx_rti_type* _type = dx_ddl_word_get_type();
  if (!_type) {
    return 1;
  }
  if (!self || !text) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_ddl_word_destruct(dx_ddl_word* self) {
  DX_UNREFERENCE(self->text);
  self->text = NULL;
}

dx_ddl_word* dx_ddl_word_create(dx_ddl_word_kind kind, dx_string* text) {
  dx_ddl_word* self = DX_DDL_WORD(dx_object_alloc(sizeof(dx_ddl_word)));
  if (!self) {
    return NULL;
  }
  if (dx_ddl_word_construct(self, kind, text)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
