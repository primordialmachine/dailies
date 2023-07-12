#include "dx/fps_counter.h"

// GetTickCount64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

DX_DEFINE_OBJECT_TYPE("dx.fps_counter",
                      dx_fps_counter,
                      dx_object)

static void dx_fps_counter_destruct(dx_fps_counter* self)
{/*Intentionally empty.*/}

int dx_fps_counter_construct(dx_fps_counter* self) {
  if (!self) {
    return 1;
  }
  dx_rti_type* _type = dx_fps_counter_get_type();
  if (!_type) {
    return 1;
  }
  self->started = 0;
  self->start = 0;
  self->write = 0;
  self->size = 0;
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_fps_counter* dx_fps_counter_create() {
  dx_fps_counter* self = DX_FPS_COUNTER(dx_object_alloc(sizeof(dx_fps_counter)));
  if (!self) {
    return NULL;
  }
  if (dx_fps_counter_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_fps_counter_on_enter_frame(dx_fps_counter* self) {
  if (!self) {
    return 1;
  }
  self->started = true;
  self->start = GetTickCount64();
  return 0;
}

int dx_fps_counter_on_leave_frame(dx_fps_counter* self) {
  if (!self) {
    return 1;
  }
  if (!self->started) {
    return 1;
  }
  dx_n64 delta = GetTickCount64() - self->start;
  self->started = false;
  self->durations[self->write] = delta;
  self->write = (self->write + 1) % 64;
  if (self->size < 64) {
    self->size++;
  }
  return 0;
}

dx_f64 dx_fps_counter_get_fps(dx_fps_counter* self) {
  if (!self->size) {
    return 0.;
  } else {
    dx_f64 average_duration = 0.;
    for (dx_size i = 0, n = self->size; i < n; ++i) {
      average_duration += (dx_f64)self->durations[i];
    }
    average_duration /= (dx_f64)self->size;
    if (average_duration == 0.f) {
      return DX_F64_GREATEST;
    }
    return 1000. / average_duration;
  }
}
