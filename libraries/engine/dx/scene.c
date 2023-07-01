#include "dx/scene.h"

DX_DEFINE_OBJECT_TYPE("dx.scene",
                      dx_scene,
                      dx_object)

int dx_scene_construct(dx_scene* self) {
  dx_rti_type* _type = dx_scene_get_type();
  if (!_type) {
    return 1;
  }
  self->startup = NULL;
  self->render = NULL;
  self->shutdown = NULL;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_scene_destruct(dx_scene* self)
{/*Intentionally empty.*/}

int dx_scene_startup(dx_scene* self, dx_context* context) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (!self->startup) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return self->startup(self, context);
}

int dx_scene_render(dx_scene* self, dx_context* context, dx_f32 delta_seconds, int canvas_width, int canvas_height) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (!self->render) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return self->render(self, context, delta_seconds, canvas_width, canvas_height);
}

int dx_scene_shutdown(dx_scene* self, dx_context* context) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (!self->shutdown) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return self->shutdown(self, context);
}
