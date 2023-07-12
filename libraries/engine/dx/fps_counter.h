#if !defined(DX_FPS_COUNTER_H_INCLUDED)
#define DX_FPS_COUNTER_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.fps_counter",
                       dx_fps_counter,
                       dx_object)

static inline dx_fps_counter* DX_FPS_COUNTER(void* p) {
  return (dx_fps_counter*)p;
}

struct dx_fps_counter {
  dx_object _parent;
  
  /// @brief List of frame durations.
  dx_n64 durations[64];
  /// @brief The position to write an incoming duration to.
  dx_size write;
  /// @brief The number of durations.
  dx_size size;
  
  /// @brief Are we inside a frame?
  bool started;
  /// @brief Point in time at which the frame started if we are inside a frame.
  dx_n64 start;
};

int dx_fps_counter_construct(dx_fps_counter* self);

dx_fps_counter* dx_fps_counter_create();

int dx_fps_counter_on_enter_frame(dx_fps_counter* self);

int dx_fps_counter_on_leave_frame(dx_fps_counter* self);

dx_f64 dx_fps_counter_get_fps(dx_fps_counter* self);

#endif // DX_FPS_COUNTER_H_INCLUDED
