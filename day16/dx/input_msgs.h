#if !defined(DX_INPUT_MSGS_H_INCLUDED)
#define DX_INPUT_MSGS_H_INCLUDED

#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_KEYBOARD_KEY_ENTER (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_INPUT_MSG_TYPE_KEYBOARD_KEY (1)

typedef struct dx_input_msg dx_input_msg;

static inline dx_input_msg* DX_INPUT_MSG(void* p) {
  return (dx_input_msg*)p;
}

int dx_input_msg_construct(dx_input_msg* input_msg, int type);

void dx_input_msg_destruct(dx_input_msg* input_msg);

int dx_input_msg_get_type(dx_input_msg* input_msg);

struct dx_input_msg {
  dx_msg _parent;
  int type;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_KEYBOARD_KEY_ACTION_PRESSED (1)

#define DX_KEYBOARD_KEY_ACTION_RELEASED (2)

// The opaque type of a "keyboard key" message
typedef struct dx_keyboard_key_msg dx_keyboard_key_msg;

static inline dx_keyboard_key_msg* DX_KEYBOARD_KEY_MSG(void* p) {
  return (dx_keyboard_key_msg*)p;
}

// Construct this keyboard key message.
// @param action The action of the key. Must be #DX_KEYBOARD_KEY_ACTION_PRESSED or #DX_KEYBOARD_KEY_ACTION_RELEASED.
// @param key The key.
int dx_keyboard_key_msg_construct(dx_keyboard_key_msg* keyboard_key_msg, int action, int key);

void dx_keyboard_key_msg_destruct(dx_keyboard_key_msg* keyboard_key_msg);

int dx_keyboard_key_msg_get_action(dx_keyboard_key_msg* keyboard_key_msg);

int dx_keyboard_key_msg_get_key(dx_keyboard_key_msg* keyboard_key_msg);

// Create a "keyboard key" message
// @param type the type of the "keyboard key" message
// @param key the key
// @return pointer to the message on success. null pointer on failure
dx_keyboard_key_msg* dx_keyboard_key_msg_create(int type, int key);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_INPUT_MSGS_H_INCLUDED
