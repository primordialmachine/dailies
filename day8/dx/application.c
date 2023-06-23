#include "dx/application.h"

#if defined(_WIN32)
  #include "dx/gl/wgl/wm.h"
#else
  #error("environment not (yet) supported")
#endif

DX_DEFINE_OBJECT_TYPE("dx.application",
                      dx_application,
                      dx_object)

int dx_application_construct(dx_application* application) {
  DX_OBJECT(application)->destruct = (void(*)(dx_object*))&dx_application_destruct;
  return 0;
}

void dx_application_destruct(dx_application* application) 
{/*Intentionally empty.*/}

int dx_application_startup(dx_msg_queue* msg_queue) {
  return dx_gl_wgl_application_startup(msg_queue);
}

void dx_application_shutdown() {
  dx_gl_wgl_application_shutdown();
}

dx_application* dx_application_get() {
  return (dx_application*)dx_gl_wgl_application_get();
}
