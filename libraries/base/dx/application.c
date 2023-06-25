#include "dx/application.h"

int dx_application_construct(dx_application* application) {
  DX_OBJECT(application)->destruct = (void(*)(dx_object*))&dx_application_destruct;
  return 0;
}

void dx_application_destruct(dx_application* application) 
{/*Intentionally empty.*/}
