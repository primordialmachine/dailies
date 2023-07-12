#include "dx/ddl/word_kind.h"

DX_DEFINE_ENUMERATION_TYPE("dx.ddl.word_kind",
                           dx_ddl_word_kind)

dx_string* dx_ddl_word_kind_to_string(dx_ddl_word_kind self) {
  switch (self) {
  #define DEFINE(SYMBOL, STRING) \
    case SYMBOL: { \
      return dx_string_create(STRING, sizeof(STRING) - 1); \
    } break;
  #include "dx/ddl/word_kind.i"
  #undef DEFINE
    default: {
      dx_set_error(DX_INVALID_ARGUMENT);
      return NULL;
    } break;
  };
}
