/// @file dx/core/hapticals.c
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

DX_DEFINE_OBJECT_TYPE("dx.input_msg",
                      dx_input_msg,
                      dx_msg);

int dx_input_msg_construct(dx_input_msg* self, dx_input_msg_kind kind) {
  dx_rti_type* _type = dx_input_msg_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_msg_construct(DX_MSG(self))) {
    return 1;
  }
  self->kind = kind;
  DX_MSG(self)->flags = DX_MSG_TYPE_INPUT;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_input_msg_destruct(dx_input_msg* self)
{/*Intentionally empty.*/}

dx_input_msg_kind dx_input_msg_get_kind(dx_input_msg* self) {
  return self->kind;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.keyboard_key_msg",
                      dx_keyboard_key_msg,
                      dx_input_msg);

int dx_keyboard_key_msg_construct(dx_keyboard_key_msg* self, uint8_t action, dx_keyboard_key key, uint8_t modifiers) {
  dx_rti_type* _type = dx_keyboard_key_msg_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_input_msg_construct(DX_INPUT_MSG(self), DX_INPUT_MSG_KIND_KEYBOARD_KEY)) {
    return 1;
  }
  self->action = action;
  self->key = key;
  self->modifiers = modifiers;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_keyboard_key_msg_destruct(dx_keyboard_key_msg* self)
{/*Intentionally empty.*/}

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

DX_DEFINE_OBJECT_TYPE("dx.mouse_button_msg",
                      dx_mouse_button_msg,
                      dx_input_msg);

int dx_mouse_button_msg_construct(dx_mouse_button_msg* self, uint8_t action, dx_mouse_button button, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  dx_rti_type* _type = dx_mouse_button_msg_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_input_msg_construct(DX_INPUT_MSG(self), DX_INPUT_MSG_KIND_MOUSE_BUTTON)) {
    return 1;
  }
  self->action = action;
  self->button = button;
  self->modifiers = modifiers;
  self->x = x;
  self->y = y;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_mouse_button_msg_destruct(dx_mouse_button_msg* self)
{/*Intentionally empty.*/}

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

DX_DEFINE_OBJECT_TYPE("dx.mouse_pointer_msg",
                      dx_mouse_pointer_msg,
                      dx_input_msg);

int dx_mouse_pointer_msg_construct(dx_mouse_pointer_msg* self, uint8_t action, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  dx_rti_type* _type = dx_mouse_pointer_msg_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_input_msg_construct(DX_INPUT_MSG(self), DX_INPUT_MSG_KIND_MOUSE_POINTER)) {
    return 1;
  }
  self->action = action;
  self->modifiers = modifiers;
  self->x = x;
  self->y = y;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_mouse_pointer_msg_destruct(dx_mouse_pointer_msg* self)
{/*Intentionally empty.*/}

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
