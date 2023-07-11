#if !defined(DX_DDL_LINEMAP_H_INCLUDED)
#define DX_DDL_LINEMAP_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.ddl.linemap",
                       dx_ddl_linemap,
                       dx_object)

static inline dx_ddl_linemap* DX_DDL_LINEMAP(void* p) {
  return (dx_ddl_linemap*)p;
}

struct dx_ddl_linemap {
  dx_object _parent;
};

int dx_ddl_linemap_construct(dx_ddl_linemap* self);

dx_ddl_linemap_create();

#endif // DX_DDL_LINEMAP_H_INCLUDED
