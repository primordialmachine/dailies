#if !defined(DX_APPLICATION_H_INCLUDED)
#define DX_APPLICATION_H_INCLUDED

#include "dx/core.h"

/// The representation of an application.
DX_DECLARE_OBJECT_TYPE("dx.application",
                       dx_application,
                       dx_object);

static dx_application* DX_APPLICATION(void* p) {
  return (dx_application*)p;
}

struct dx_application {
  dx_object _parent;
  int (*update)(dx_application* self);
};

int dx_application_construct(dx_application* self);

int dx_application_update(dx_application* self);

#endif // DX_APPLICATION_H_INCLUDED
