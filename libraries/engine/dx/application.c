#include "dx/application.h"

DX_DEFINE_OBJECT_TYPE("dx.application",
                      dx_application,
                      dx_object);

int dx_application_construct(dx_application* self) {
  dx_rti_type* _type = dx_application_get_type();
  if (!_type) {
    return 1;
  }
  self->update = NULL;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_application_destruct(dx_application* self)
{/*Intentionally empty.*/}

int dx_application_update(dx_application* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
#if defined(_DEBUG)
  if (!self->update) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
#endif
  return self->update(self);
}
