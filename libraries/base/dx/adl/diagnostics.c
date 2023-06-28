#include "dx/adl/diagnostics.h"

DX_DEFINE_OBJECT_TYPE("dx.adl.diagnostics",
                      dx_adl_diagnostics,
                      dx_object)

int dx_adl_diagnostics_construct(dx_adl_diagnostics* self) {
  dx_rti_type* _type = dx_adl_diagnostics_get_type();
  if (!_type) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_diagnostics_destruct(dx_adl_diagnostics* self)
{/*Intentionally empty.*/}

dx_adl_diagnostics* dx_adl_diagnostics_create() {
  dx_adl_diagnostics* self = DX_ADL_DIAGNOSTICS(dx_object_alloc(sizeof(dx_adl_diagnostics)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_diagnostics_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_adl_diagnostics_on_lexical(dx_adl_diagnostics* self) {
  dx_string* format = dx_string_create("lexical error:\n", sizeof("lexical error:\n"));
  if (!format) {
    return 1;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  return 0;
}

int dx_adl_diagnostics_on_syntactical(dx_adl_diagnostics* self) {
  dx_string* format = dx_string_create("syntactical error:\n", sizeof("syntactical error:\n"));
  if (!format) {
    return 1;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  return 0;
}

int dx_adl_diagnostics_on_semantical(dx_adl_diagnostics* self) {
  dx_string* format = dx_string_create("semantical error:\n", sizeof("semantical error:\n"));
  if (!format) {
    return 1;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  return 0;
}
