#if !defined(DX_ADL_DIAGNOSTICS_H_INCLUDED)
#define DX_ADL_DIAGNOSTICS_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.diagnostics",
                       dx_adl_diagnostics,
                       dx_object)

static inline dx_adl_diagnostics* DX_ADL_DIAGNOSTICS(void* p) {
  return (dx_adl_diagnostics*)p;
}

struct dx_adl_diagnostics {
  dx_object _parent;
};

int dx_adl_diagnostics_construct(dx_adl_diagnostics* self);

dx_adl_diagnostics* dx_adl_diagnostics_create();

int dx_adl_diagnostics_on_lexical(dx_adl_diagnostics* self);

int dx_adl_diagnostics_on_syntactical(dx_adl_diagnostics* self);

int dx_adl_diagnostics_on_semantical(dx_adl_diagnostics* self);

#endif // DX_ADL_DIAGNOSTICS_H_INCLUDED
