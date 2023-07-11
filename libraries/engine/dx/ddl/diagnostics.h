/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DDL_DIAGNOSTICS_H_INCLUDED)
#define DX_DDL_DIAGNOSTICS_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.ddl.diagnostics",
                       dx_ddl_diagnostics,
                       dx_object)

static inline dx_ddl_diagnostics* DX_DDL_DIAGNOSTICS(void* p) {
  return (dx_ddl_diagnostics*)p;
}

struct dx_ddl_diagnostics {
  dx_object _parent;
};

int dx_ddl_diagnostics_construct(dx_ddl_diagnostics* self);

dx_ddl_diagnostics* dx_ddl_diagnostics_create();

int dx_ddl_diagnostics_on_lexical(dx_ddl_diagnostics* self);

int dx_ddl_diagnostics_on_unclosed_string_literal(dx_ddl_diagnostics* self);

int dx_ddl_diagnostics_on_invalid_number_literal(dx_ddl_diagnostics* self);

int dx_ddl_diagnostics_on_unclosed_multi_line_comment(dx_ddl_diagnostics* self);

int dx_ddl_diagnostics_on_unexpected_symbol(dx_ddl_diagnostics* self);

#endif // DX_DDL_DIAGNOSTICS_H_INCLUDED
