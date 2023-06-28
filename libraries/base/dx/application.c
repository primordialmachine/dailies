#include "dx/application.h"

DX_DEFINE_OBJECT_TYPE("dx.application",
                      dx_application,
                      dx_object);

int dx_application_construct(dx_application* self) {
  dx_rti_type* _type = dx_application_get_type();
  if (!_type) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_application_destruct(dx_application* self)
{/*Intentionally empty.*/}
