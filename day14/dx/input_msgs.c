#include "dx/input_msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_input_msg_construct(dx_input_msg* input_msg, int type) {
  if (dx_msg_construct(DX_MSG(input_msg))) {
    return 1;
  }
  input_msg->type = type;
  DX_MSG(input_msg)->flags = DX_MSG_TYPE_INPUT;
  DX_OBJECT(input_msg)->destruct = (void (*)(dx_object*)) & dx_input_msg_destruct;
  return 0;
}

void dx_input_msg_destruct(dx_input_msg* input_msg) {
  dx_msg_destruct(DX_MSG(input_msg));
}

int dx_input_msg_get_type(dx_input_msg* input_msg) {
  return input_msg->type;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct dx_keyboard_key_msg {
  dx_input_msg _parent;
  int action;
  int key;
};

int dx_keyboard_key_msg_construct(dx_keyboard_key_msg* keyboard_key_msg, int action, int key) {
  if (dx_input_msg_construct(DX_INPUT_MSG(keyboard_key_msg), DX_INPUT_MSG_TYPE_KEYBOARD_KEY)) {
    return 1;
  }
  keyboard_key_msg->action = action;
  keyboard_key_msg->key = key;
  DX_OBJECT(keyboard_key_msg)->destruct = (void (*)(dx_object*)) & dx_keyboard_key_msg_destruct;
  return 0;
}

void dx_keyboard_key_msg_destruct(dx_keyboard_key_msg* keyboard_key_msg) {
  dx_input_msg_destruct(DX_INPUT_MSG(keyboard_key_msg));
}

int dx_keyboard_key_msg_get_action(dx_keyboard_key_msg* keyboard_key_msg) {
  return keyboard_key_msg->action;
}

int dx_keyboard_key_msg_get_key(dx_keyboard_key_msg* keyboard_key_msg) {
  return keyboard_key_msg->key;
}

dx_keyboard_key_msg* dx_keyboard_key_msg_create(int action, int key) {
  dx_keyboard_key_msg* keyboard_key_msg = DX_KEYBOARD_KEY_MSG(dx_object_alloc(sizeof(dx_keyboard_key_msg)));
  if (!keyboard_key_msg) {
    return NULL;
  }
  if (dx_keyboard_key_msg_construct(keyboard_key_msg, action, key)) {
    DX_UNREFERENCE(keyboard_key_msg);
    keyboard_key_msg = NULL;
    return NULL;
  }
  return keyboard_key_msg;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
