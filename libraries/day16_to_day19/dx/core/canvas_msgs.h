#if !defined(DX_CANVAS_H_INCLUDED)
#define DX_CANVAS_H_INCLUDED

#include "dx/core/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef enum dx_canvas_msg_type {
  dx_canvas_msg_type_activated,
  dx_canvas_msg_type_deactivated,
  dx_canvas_msg_type_size_changed,
} dx_canvas_msg_type;

typedef struct dx_canvas_msg dx_canvas_msg;
static inline dx_canvas_msg* DX_CANVAS_MSG(void* p) {
  return (dx_canvas_msg*)p;
}

struct dx_canvas_msg {
  dx_msg _parent;
  uint8_t type;
};

int dx_canvas_msg_construct(dx_canvas_msg* self, dx_canvas_msg_type type);

void dx_canvas_msg_destruct(dx_canvas_msg* self);

dx_canvas_msg* dx_canvas_msg_create(dx_canvas_msg_type type);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct dx_canvas_size_changed_msg dx_canvas_size_changed_msg;
static inline dx_canvas_size_changed_msg* DX_CANVAS_SIZE_CHANGED_MSG(void* p) {
  return (dx_canvas_size_changed_msg*)p;
}

struct dx_canvas_size_changed_msg {
  dx_canvas_msg _parent;
  dx_f32 width;
  dx_f32 height;
};

int dx_canvas_size_changed_msg_construct(dx_canvas_size_changed_msg* self, dx_f32 width, dx_f32 height);

void dx_canvas_size_changed_msg_destruct(dx_canvas_size_changed_msg* self);

dx_canvas_size_changed_msg* dx_canvas_size_changed_msg_create(dx_f32 width, dx_f32 height);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CANVAS_H_INCLUDED
