#include "dx/core/msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/memory.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MSG_TRACE (0)

#if defined(DX_MSG_TRACE) && 1 == DX_MSG_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

DX_DEFINE_OBJECT_TYPE("dx.msg",
                      dx_msg,
                      dx_object);

uint32_t dx_msg_get_flags(dx_msg const* msg) {
  return msg->flags;
}

int dx_msg_construct(dx_msg* msg) {
  TRACE("enter: dx_msg_construct\n");
  dx_rti_type* _type = dx_msg_get_type();
  if (!_type) {
    return 1;
  }
  msg->flags = DX_MSG_TYPE_UNDETERMINED;
  DX_OBJECT(msg)->type = _type;
  TRACE("leave: dx_msg_construct\n");
  return 0;
}

static void dx_msg_destruct(dx_msg* msg)
{/*Intentionally empty.*/}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_EMIT_MSG_TRACE (0)

#if defined(DX_EMIT_MSG_TRACE) && 1 == DX_EMIT_MSG_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

DX_DEFINE_OBJECT_TYPE("dx.emit_msg",
                      dx_emit_msg,
                      dx_msg);

int dx_emit_msg_construct(dx_emit_msg* self, char const* p, dx_size n) {
  TRACE("enter: dx_emit_msg_construct\n");
  dx_rti_type* _type = dx_emit_msg_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_msg_construct(DX_MSG(self))) {
    TRACE("leave: dx_emit_msg_construct\n");
    return 1;
  }
  self->p = dx_memory_allocate(n);
  if (!self->p) {
    TRACE("leave: dx_emit_msg_construct\n");
    return 1;
  }
  dx_memory_copy(self->p, p, n);
  self->n = n;
  DX_MSG(self)->flags = DX_MSG_TYPE_EMIT;
  DX_OBJECT(self)->type = _type;
  TRACE("leave: dx_emit_msg_construct\n");
  return 0;
}

static void dx_emit_msg_destruct(dx_emit_msg* emit_msg) {
  TRACE("enter: dx_emit_msg_destruct\n");
  dx_memory_deallocate(emit_msg->p);
  emit_msg->p = NULL;
  TRACE("leave: dx_emit_msg_destruct\n");
}

dx_emit_msg* dx_emit_msg_create(char const* p, dx_size n) {
  TRACE("enter: dx_emit_msg_create\n");
  dx_emit_msg* self = DX_EMIT_MSG(dx_object_alloc(sizeof(dx_emit_msg)));
  if (!self) {
    TRACE("leave: dx_emit_msg_create\n");
    return NULL;
  }
  if (dx_emit_msg_construct(self, p, n)) {
    DX_UNREFERENCE(self);
    self = NULL;
    TRACE("leave: dx_emit_msg_create\n");
    return NULL;
  }
  TRACE("leave: dx_emit_msg_create\n");
  return self;
}

int dx_emit_msg_get(dx_emit_msg* self, char const** p, size_t* n) {
  *p = self->p;
  *n = self->n;
  return 0;
}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_QUIT_MSG_TRACE (0)

#if defined(DX_QUIT_MSG_TRACE) && 1 == DX_QUIT_MSG_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

DX_DEFINE_OBJECT_TYPE("dx.quit_msg",
                      dx_quit_msg,
                      dx_msg);

int dx_quit_msg_construct(dx_quit_msg* self) {
  TRACE("enter: dx_quit_msg_construct\n");
  dx_rti_type* _type = dx_quit_msg_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_msg_construct(DX_MSG(self))) {
    TRACE("leave: dx_quit_msg_construct\n");
    return 1;
  }
  DX_MSG(self)->flags = DX_MSG_TYPE_QUIT;
  DX_OBJECT(self)->type = _type;
  TRACE("leave: dx_quit_msg_construct\n");
  return 0;
}

static void dx_quit_msg_destruct(dx_quit_msg* self)
{/*Intentionally empty.*/}

dx_quit_msg* dx_quit_msg_create() {
  TRACE("enter: dx_quit_msg_create\n");
  dx_quit_msg* self = DX_QUIT_MSG(dx_object_alloc(sizeof(dx_quit_msg)));
  if (!self) {
    TRACE("leave: dx_quit_msg_create\n");
    return NULL;
  }
  if (dx_quit_msg_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    TRACE("leave: dx_quit_msg_create\n");
    return NULL;
  }
  TRACE("leave: dx_quit_msg_create\n");
  return self;
}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MSG_QUEUE_TRACE (0)

#if defined(DX_MSG_QUEUE_TRACE) && 1 == DX_MSG_QUEUE_TRACE
  #define TRACE(msg) dx_log(msg, sizeof(msg) - 1)
#else
  #define TRACE(msg)
#endif

typedef struct dx_msg_queue {
  dx_msg** elements;
  dx_size size, capacity;
  dx_size write, read;
} dx_msg_queue;

int dx_msg_queue_push(dx_msg_queue* msg_queue, dx_msg* msg) {
  TRACE("enter: dx_msg_queue_push\n");
  if (!msg_queue || !msg) {
    dx_set_error(DX_INVALID_ARGUMENT);
    TRACE("leave: dx_msg_queue_push\n");
    return 1;
  }
  if (msg_queue->size == msg_queue->capacity) {
    dx_set_error(DX_ALLOCATION_FAILED);
    TRACE("leave: dx_msg_queue_push (failure)\n");
    return 1;
  }
  msg_queue->elements[msg_queue->write] = msg;
  msg_queue->write = (msg_queue->write + 1) % msg_queue->capacity;
  msg_queue->size++;
  DX_REFERENCE(msg);
  TRACE("leave: dx_msg_queue_push (success)\n");
  return 0;
}

int dx_msg_queue_pop(dx_msg_queue* msg_queue, dx_msg** msg) {
  TRACE("enter: dx_msg_queue_pop\n");
  if (msg_queue->size == 0) {
    *msg = NULL;
    TRACE("leave: dx_msg_queue_pop (success)\n");
    return 0;
  } else {
    *msg = msg_queue->elements[msg_queue->read];
    msg_queue->read = (msg_queue->read + 1) % msg_queue->capacity;
    msg_queue->size--;
    TRACE("leave: dx_msg_queue_pop (success)\n");
    return 0;
  }
}

dx_msg_queue* dx_msg_queue_create() {
  TRACE("enter: dx_msg_queue_create\n");
  dx_msg_queue* msg_queue = dx_memory_allocate(sizeof(dx_msg_queue));
  if (!msg_queue) {
    dx_log("allocation failed\n", sizeof("allocation failed\n"));
    TRACE("leave: dx_msg_queue_create\n");
    return NULL;
  }
  msg_queue->size = 0;
  msg_queue->capacity = 1024;
  msg_queue->write = 0;
  msg_queue->read = 0;
  msg_queue->elements = dx_memory_allocate(sizeof(dx_msg*) * 1024);
  if (!msg_queue->elements) {
    dx_log("allocation failed\n", sizeof("allocation failed\n"));
    dx_memory_deallocate(msg_queue);
    msg_queue = NULL;
    TRACE("leave: dx_msg_queue_create (failure)\n");
    return NULL;
  }
  TRACE("leave: dx_msg_queue_create (success)\n");
  return msg_queue;
}

void dx_msg_queue_destroy(dx_msg_queue* msg_queue) {
  TRACE("enter: dx_msg_queue_destroy\n");
  if (msg_queue->elements) {
    while (msg_queue->size > 0) {
      dx_msg* msg = msg_queue->elements[--msg_queue->size];
      DX_UNREFERENCE(msg);
    }
    dx_memory_deallocate(msg_queue->elements);
    msg_queue->elements = NULL;
  }
  dx_memory_deallocate(msg_queue);
  msg_queue = NULL;
  TRACE("leave: dx_msg_queue_destroy\n");
}

#undef TRACE

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
