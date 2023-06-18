#if !defined(DX_APPLICATION_H_INCLUDED)
#define DX_APPLICATION_H_INCLUDED

#include "dx/core.h"

/// The representation of an application.
typedef struct dx_application dx_application;

static dx_application* DX_APPLICATION(void* p) {
  return (dx_application*)p;
}

struct dx_application {
  dx_object _parent;
};

int dx_application_construct(dx_application* application);

void dx_application_destruct(dx_application* application);

#endif // DX_APPLICATION_H_INCLUDED
