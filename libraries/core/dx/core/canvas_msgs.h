#if !defined(DX_CANVAS_H_INCLUDED)
#define DX_CANVAS_H_INCLUDED

#include "dx/core/msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("dx.canvas_msg_kind",
                            dx_canvas_msg_kind);

enum dx_canvas_msg_kind {
  dx_canvas_msg_kind_activated,
  dx_canvas_msg_kind_deactivated,
  dx_canvas_msg_kind_size_changed,
};

DX_DECLARE_OBJECT_TYPE("dx.canvas_msg",
                       dx_canvas_msg,
                       dx_msg)

static inline dx_canvas_msg* DX_CANVAS_MSG(void* p) {
  return (dx_canvas_msg*)p;
}

int dx_canvas_msg_construct(dx_canvas_msg* self, dx_canvas_msg_kind kind);

void dx_canvas_msg_destruct(dx_canvas_msg* self);

dx_canvas_msg* dx_canvas_msg_create(dx_canvas_msg_kind kind);

struct dx_canvas_msg {
  dx_msg _parent;
  uint8_t kind;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.canvas_size_changed_msg",
                       dx_canvas_size_changed_msg,
                       dx_canvas_msg)

static inline dx_canvas_size_changed_msg* DX_CANVAS_SIZE_CHANGED_MSG(void* p) {
  return (dx_canvas_size_changed_msg*)p;
}

int dx_canvas_size_changed_msg_construct(dx_canvas_size_changed_msg* self, dx_f32 width, dx_f32 height);

void dx_canvas_size_changed_msg_destruct(dx_canvas_size_changed_msg* self);

dx_canvas_size_changed_msg* dx_canvas_size_changed_msg_create(dx_f32 width, dx_f32 height);

struct dx_canvas_size_changed_msg {
  dx_canvas_msg _parent;
  dx_f32 width;
  dx_f32 height;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CANVAS_H_INCLUDED
