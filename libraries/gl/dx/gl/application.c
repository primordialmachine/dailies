#include "dx/gl/application.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.application",
                      dx_gl_application,
                      dx_application)

int dx_gl_application_construct(dx_gl_application* self, dx_msg_queue* msg_queue) {
  dx_rti_type* _type = dx_gl_application_get_type();
  if (!_type) {
    return 1;
  }
  //
  if (dx_application_construct(DX_APPLICATION(self))) {
    return 1;
  }
  //
  self->msg_queue = msg_queue;
  //
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_gl_application_destruct(dx_gl_application* self)
{/*Intentionally empty.*/}
