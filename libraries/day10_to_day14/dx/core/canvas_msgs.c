#include "dx/core/canvas_msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_canvas_msg_construct(dx_canvas_msg* self, dx_canvas_msg_type type) {
  if (dx_msg_construct(DX_MSG(self))) {
    return 1;
  }
  self->type = type;
  DX_MSG(self)->flags = DX_MSG_TYPE_CANVAS;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_canvas_msg_destruct;
  return 0;
}

void dx_canvas_msg_destruct(dx_canvas_msg* self) {
  dx_msg_destruct(DX_MSG(self));
}

dx_canvas_msg* dx_canvas_msg_create(dx_canvas_msg_type type) {
  dx_canvas_msg* self = DX_CANVAS_MSG(dx_object_alloc(sizeof(dx_canvas_msg)));
  if (!self) {
    return NULL;
  }
  if (dx_canvas_msg_construct(self, type)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_canvas_size_changed_msg_construct(dx_canvas_size_changed_msg* self, dx_f32 width, dx_f32 height) {
  if (dx_canvas_msg_construct(DX_CANVAS_MSG(self), dx_canvas_msg_type_size_changed)) {
    return 1;
  }
  self->width = width;
  self->height = height;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_canvas_size_changed_msg_destruct;
  return 0;
}

void dx_canvas_size_changed_msg_destruct(dx_canvas_size_changed_msg* self) {
  dx_canvas_msg_destruct(DX_CANVAS_MSG(self));
}

dx_canvas_size_changed_msg* dx_canvas_size_changed_msg_create(dx_f32 width, dx_f32 height) {
  dx_canvas_size_changed_msg* self = DX_CANVAS_SIZE_CHANGED_MSG(dx_object_alloc(sizeof(dx_canvas_size_changed_msg)));
  if (!self) {
    return NULL;
  }
  if (dx_canvas_size_changed_msg_construct(self, width, height)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/