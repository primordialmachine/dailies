#if !defined(DX_CORE_MSGS_H_INCLUDED)
#define DX_CORE_MSGS_H_INCLUDED

#include "dx/core/object.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MSG_TYPE_UNDETERMINED (0)

#define DX_MSG_TYPE_EMIT (1)

#define DX_MSG_TYPE_QUIT (2)

/// @brief Messages related to the canvas.
#define DX_MSG_TYPE_CANVAS (3)

#define DX_MSG_TYPE_INPUT (4)

// The opaque type of a message.
DX_DECLARE_OBJECT_TYPE("dx.msg",
                       dx_msg,
                       dx_object);

static inline dx_msg* DX_MSG(void* p) {
  return (dx_msg*)p;
}

uint32_t dx_msg_get_flags(dx_msg const* msg);

int dx_msg_construct(dx_msg* msg);

/// @brief Destruct this message.
/// @param msg A pointer to this message.
void dx_msg_destruct(dx_msg* msg);

struct dx_msg {
  dx_object _parent;
  uint32_t flags;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of an "emit" message.
DX_DECLARE_OBJECT_TYPE("dx.emit_msg",
                       dx_emit_msg,
                       dx_msg);

static inline dx_emit_msg* DX_EMIT_MSG(void* p) {
  return (dx_emit_msg*)p;
}

int dx_emit_msg_construct(dx_emit_msg* emit_msg, char const* p, dx_size n);

void dx_emit_msg_destruct(dx_emit_msg* emit_msg);

// Create an "emit" message.
// @param p Pointer to an utf-8 string of @a n Bytes.
// @param n the length of the utf-8 string in Bytes pointed to by @a p.
// @return A pointer to the message on success. The null pointer on failure.
dx_emit_msg* dx_emit_msg_create(char const* p, dx_size n);

int dx_emit_msg_get(dx_emit_msg* emit_msg, char const** p, dx_size* n);

struct dx_emit_msg {
  dx_msg _parent;
  char* p;
  dx_size n;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of an "quit" message.
DX_DECLARE_OBJECT_TYPE("dx.quit_msg",
                       dx_quit_msg,
                       dx_msg);

static inline dx_quit_msg* DX_QUIT_MSG(void* p) {
  return (dx_quit_msg*)p;
}

int dx_quit_msg_construct(dx_quit_msg* quit_msg);

void dx_quit_msg_destruct(dx_quit_msg* emit_msg);

// Create a "quit" message
// @return pointer to the message on success. null pointer on failure
dx_quit_msg* dx_quit_msg_create();

struct dx_quit_msg {
  dx_msg _parent;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// The opaque type of a message queue.
typedef struct dx_msg_queue dx_msg_queue;

// Push a message on the message queue.
// @post
// On success:
// The message was added to the queue and the queue acquired a reference to the message.
// @return @a 0 on success. A non-zero value on failure.
int dx_msg_queue_push(dx_msg_queue* msg_queue, dx_msg* msg);

// Pop a message from the message queue.
// @post
// On success:
// <code>*msg</code> was assigned a pointer to the message if the queue is not empty.
// The reference acquired to the message when it was added to the message queue was transferred to the caller.
// It was assigned the null pointer otherwise.
// On failure:
// <code>msg</code> is not dereferenced.
// @return
// @a 0 on success. A non-zero value on failure
int dx_msg_queue_pop(dx_msg_queue* msg_queue, dx_msg** msg);

// Create a message queue.
// @return pointer to the message queue on success. null pointer on failure.
dx_msg_queue* dx_msg_queue_create();

// Destroy a message queue.
// The message queue relinquishes all references to messages contained in the queue.
void dx_msg_queue_destroy(dx_msg_queue* msg_queue);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_MSGS_H_INCLUDED