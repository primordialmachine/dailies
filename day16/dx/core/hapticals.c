/// @file dx/core/keyboard/hapticals.c
/// @brief Mouse buttons and keyboard keys.
#include "dx/core/hapticals.h"

#include <stdio.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

char const* dx_keyboard_key_to_string(dx_keyboard_key self) {
  //if (Nucleus_Unlikely(!string)) return Nucleus_Status_InvalidArgument;
  switch (self) {
  #define ALIAS(aliased,alias)
  #define DEFINE(name,value,description)\
    case dx_keyboard_key_##name: {\
      static const char *STRING = description;\
      return STRING;\
    };
  #include "dx/core/keyboard_keys.i"
  #undef DEFINE
  #undef ALIAS
    default: {
      dx_set_error(DX_INVALID_ARGUMENT);
      return NULL;
    };
  };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

char const* dx_mouse_button_to_string(dx_mouse_button self) {
  switch (self) {
  #define ALIAS(aliased,alias)
  #define DEFINE(name,value,description)\
    case dx_mouse_button_##name: {\
      static const char *STRING = description;\
      return STRING;\
    };
  #include "dx/core/mouse_buttons.i"
  #undef DEFINE
  #undef ALIAS
    default: {
      dx_set_error(DX_INVALID_ARGUMENT);
      return NULL;
    };
  };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_input_msg_construct(dx_input_msg* self, int type) {
  if (dx_msg_construct(DX_MSG(self))) {
    return 1;
  }
  self->type = type;
  DX_MSG(self)->flags = DX_MSG_TYPE_INPUT;
  DX_OBJECT(self)->destruct = (void (*)(dx_object*)) & dx_input_msg_destruct;
  return 0;
}

void dx_input_msg_destruct(dx_input_msg* self) {
  dx_msg_destruct(DX_MSG(self));
}

int dx_input_msg_get_type(dx_input_msg* self) {
  return self->type;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct dx_keyboard_key_msg {
  dx_input_msg _parent;
  uint8_t action;
  dx_keyboard_key key;
  uint8_t modifiers;
};

int dx_keyboard_key_msg_construct(dx_keyboard_key_msg* self, uint8_t action, dx_keyboard_key key, uint8_t modifiers) {
  if (dx_input_msg_construct(DX_INPUT_MSG(self), DX_INPUT_MSG_TYPE_KEYBOARD_KEY)) {
    return 1;
  }
  self->action = action;
  self->key = key;
  self->modifiers = modifiers;
  DX_OBJECT(self)->destruct = (void (*)(dx_object*)) & dx_keyboard_key_msg_destruct;
  return 0;
}

void dx_keyboard_key_msg_destruct(dx_keyboard_key_msg* self) {
  dx_input_msg_destruct(DX_INPUT_MSG(self));
}

uint8_t dx_keyboard_key_msg_get_action(dx_keyboard_key_msg* self) {
  return self->action;
}

dx_keyboard_key dx_keyboard_key_msg_get_key(dx_keyboard_key_msg* self) {
  return self->key;
}

dx_keyboard_key_msg* dx_keyboard_key_msg_create(uint8_t action, dx_keyboard_key key, uint8_t modifiers) {
  dx_keyboard_key_msg* self = DX_KEYBOARD_KEY_MSG(dx_object_alloc(sizeof(dx_keyboard_key_msg)));
  if (!self) {
    return NULL;
  }
  if (dx_keyboard_key_msg_construct(self, action, key, modifiers)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct dx_mouse_button_msg {
  dx_input_msg _parent;
  uint8_t action;
  dx_mouse_button button;
  uint8_t modifiers;
  dx_f32 x, y;
};

int dx_mouse_button_msg_construct(dx_mouse_button_msg* self, uint8_t action, dx_mouse_button button, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  if (dx_input_msg_construct(DX_INPUT_MSG(self), DX_INPUT_MSG_TYPE_MOUSE_BUTTON)) {
    return 1;
  }
  self->action = action;
  self->button = button;
  self->modifiers = modifiers;
  self->x = x;
  self->y = y;
  DX_OBJECT(self)->destruct = (void (*)(dx_object*)) & dx_mouse_button_msg_destruct;
  return 0;
}

void dx_mouse_button_msg_destruct(dx_mouse_button_msg* self) {
  dx_input_msg_destruct(DX_INPUT_MSG(self));
}

uint8_t dx_mouse_button_msg_get_action(dx_mouse_button_msg* self) {
  return self->action;
}

dx_mouse_button dx_mouse_button_msg_get_button(dx_mouse_button_msg* self) {
  return self->button;
}

dx_mouse_button_msg* dx_mouse_button_msg_create(uint8_t action, dx_mouse_button button, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  dx_mouse_button_msg* self = DX_MOUSE_BUTTON_MSG(dx_object_alloc(sizeof(dx_mouse_button_msg)));
  if (!self) {
    return NULL;
  }
  if (dx_mouse_button_msg_construct(self, action, button, modifiers, x, y)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct dx_mouse_pointer_msg {
  dx_input_msg _parent;
  uint8_t action;
  uint8_t modifiers;
  dx_f32 x, y;
};

int dx_mouse_pointer_msg_construct(dx_mouse_pointer_msg* self, uint8_t action, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  if (dx_input_msg_construct(DX_INPUT_MSG(self), DX_INPUT_MSG_TYPE_MOUSE_POINTER)) {
    return 1;
  }
  self->action = action;
  self->modifiers = modifiers;
  self->x = x;
  self->y = y;
  DX_OBJECT(self)->destruct = (void (*)(dx_object*)) & dx_mouse_pointer_msg_destruct;
  return 0;
}

void dx_mouse_pointer_msg_destruct(dx_mouse_pointer_msg* self) {
  dx_input_msg_destruct(DX_INPUT_MSG(self));
}

uint8_t dx_mouse_pointer_msg_get_action(dx_mouse_pointer_msg* self) {
  return self->action;
}

dx_mouse_pointer_msg* dx_mouse_pointer_msg_create(uint8_t action, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  dx_mouse_pointer_msg* self = DX_MOUSE_POINTER_MSG(dx_object_alloc(sizeof(dx_mouse_pointer_msg)));
  if (!self) {
    return NULL;
  }
  if (dx_mouse_pointer_msg_construct(self, action, modifiers, x, y)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
