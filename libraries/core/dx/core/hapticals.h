/// @file dx/core/hapticals.h
/// @brief Mouse buttons and keyboard keys.
#if !defined(DX_CORE_HAPTICALS_H_INCLUDED)
#define DX_CORE_HAPTICALS_H_INCLUDED

#include "dx/core/msgs.h"

/// @ingroup core-hapticals
/// @brief The "lmenu" modifier.
#define dx_modifier_lmenu (1)
/// @ingroup core-hapticals
/// @brief The "rmenu" modifier.
#define dx_modifier_rmenu (2)

/// @ingroup core-hapticals
/// @brief The "lshift" modifier.
#define dx_modifier_lshift (4)
/// @ingroup core-hapticals
/// @brief The "rshift" modifier.
#define dx_modifier_rshift (8)

/// @ingroup core-hapticals
/// @brief The "lctrl" modifier.
#define dx_modifier_lctrl (16)
/// @ingroup core-hapticals
/// @brief The "rctrl" modifier.
#define dx_modifier_rctrl (32)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core-hapticals
/// @brief Enumeration of keyboard keys.
typedef enum dx_keyboard_key {
  #define ALIAS(aliased,alias) dx_keyboard_key_##alias = dx_keyboard_key_##aliased,
  #define DEFINE(name,value,description) dx_keyboard_key_##name = value,
  #include "dx/core/keyboard_keys.i"
  #undef DEFINE
  #undef ALIAS
} dx_keyboard_key;

/// @ingroup core-hapticals
/// @brief Return a human-readable, static constant C string describing a keyboard key.
/// @param self The keyboard key.
/// @return A pointer to a C string as described above on success. The null pointer on failure.
char const* dx_keyboard_key_to_string(dx_keyboard_key self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core-hapticals
/// @brief Enumeration of mouse buttons.
typedef enum dx_mouse_button {
#define ALIAS(aliased,alias) dx_mouse_button_##alias = dx_mouse_button_##aliased,
#define DEFINE(name,value,description) dx_mouse_button_##name = value,
#include "dx/core/mouse_buttons.i"
#undef DEFINE
#undef ALIAS
} dx_mouse_button;

/// @ingroup core-hapticals
/// @brief Return a human-readable, static constant C string describing a mouse button.
/// @param self The mouse button.
/// @return A pointer to a C string as described above on success. The null pointer on failure.
char const* dx_mouse_button_to_string(dx_mouse_button self);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_INPUT_MSG_TYPE_KEYBOARD_KEY (1)

#define DX_INPUT_MSG_TYPE_MOUSE_BUTTON (2)

#define DX_INPUT_MSG_TYPE_MOUSE_POINTER (3)

typedef struct dx_input_msg dx_input_msg;
static inline dx_input_msg* DX_INPUT_MSG(void* p) {
  return (dx_input_msg*)p;
}

/// @brief Construct this "input" message.
/// @param self A pointer to this "input" message object.
/// @param type The type of this input message.
/// Must be one of #DX_INPUT_MSG_TYPE_KEYBOARD_KEY, #DX_INPUT_MSG_TYPE_MOUSE_BUTTON, or #DX_INPUT_MSG_TYPE_MOUSE_POINTER.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
int dx_input_msg_construct(dx_input_msg* self, int type);

void dx_input_msg_destruct(dx_input_msg* self);

int dx_input_msg_get_type(dx_input_msg* self);

struct dx_input_msg {
  dx_msg _parent;
  int type;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_KEYBOARD_KEY_ACTION_PRESSED (1)

#define DX_KEYBOARD_KEY_ACTION_RELEASED (2)

/// @brief The opaque type of a "keyboard key" message
typedef struct dx_keyboard_key_msg dx_keyboard_key_msg;

static inline dx_keyboard_key_msg* DX_KEYBOARD_KEY_MSG(void* p) {
  return (dx_keyboard_key_msg*)p;
}

/// @brief Construct this "keyboard key" message.
/// @param action The action of the key. Must be #DX_KEYBOARD_KEY_ACTION_PRESSED or #DX_KEYBOARD_KEY_ACTION_RELEASED.
/// @param key The key.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
int dx_keyboard_key_msg_construct(dx_keyboard_key_msg* self, uint8_t action, dx_keyboard_key key, uint8_t modifiers);

void dx_keyboard_key_msg_destruct(dx_keyboard_key_msg* self);

uint8_t dx_keyboard_key_msg_get_action(dx_keyboard_key_msg* self);

dx_keyboard_key dx_keyboard_key_msg_get_key(dx_keyboard_key_msg* self);

/// @brief Create a "keyboard key" message.
/// @param action The action of the key. Must be #DX_KEYBOARD_KEY_ACTION_PRESSED or #DX_KEYBOARD_KEY_ACTION_RELEASED.
/// @param key The key.
/// @param modifiers The modifiers.
/// @return A pointer to the message on success. The null pointer on failure.
dx_keyboard_key_msg* dx_keyboard_key_msg_create(uint8_t action, dx_keyboard_key key, uint8_t modifiers);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MOUSE_BUTTON_ACTION_PRESSED (1)

#define DX_MOUSE_BUTTON_ACTION_RELEASED (2)

/// @brief The opaque type of a "mouse button" message
typedef struct dx_mouse_button_msg dx_mouse_button_msg;

static inline dx_mouse_button_msg* DX_MOUSE_BUTTON_MSG(void* p) {
  return (dx_mouse_button_msg*)p;
}

/// @brief Construct this "mouse button" message.
/// @param action The action of the button. Must be #DX_MOUSE_BUTTON_ACTION_PRESSED or #DX_MOUSE_BUTTON_ACTION_RELEASED.
/// @param button The button.
/// @param modifiers The modifiers.
/// @param x,y The position of the mouse pointer.
/// The origin is at the left/top of the canvas. The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @return The zero value on success. A non-zero value on failure.
int dx_mouse_button_msg_construct(dx_mouse_button_msg* self, uint8_t action, dx_mouse_button button, uint8_t modifiers, dx_f32 x, dx_f32 y);

void dx_mouse_button_msg_destruct(dx_mouse_button_msg* self);

uint8_t dx_mouse_button_msg_get_action(dx_mouse_button_msg* self);

dx_mouse_button dx_mouse_button_msg_get_button(dx_mouse_button_msg* self);

/// @brief Create a "mouse button" message.
/// @param action The action of the button. Must be #DX_MOUSE_BUTTON_ACTION_PRESSED or #DX_MOUSE_BUTTON_ACTION_RELEASED.
/// @param button The button.
/// @param modifiers The modifiers.
/// @param x,y The position of the mouse pointer.
/// The origin is at the left/top of the canvas. The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @return A pointer to the message on success. The null pointer on failure.
dx_mouse_button_msg* dx_mouse_button_msg_create(uint8_t action, dx_mouse_button button, uint8_t modifiers, dx_f32 x, dx_f32 y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define DX_MOUSE_POINTER_ACTION_MOVED (1)
#define DX_MOUSE_POINTER_ACTION_ENTERED (2)
#define DX_MOUSE_POINTER_ACTION_EXITED (3)

/// @brief The opaque type of a "mouse pointer" message
typedef struct dx_mouse_pointer_msg dx_mouse_pointer_msg;

static inline dx_mouse_pointer_msg* DX_MOUSE_POINTER_MSG(void* p) {
  return (dx_mouse_pointer_msg*)p;
}

/// @brief Construct this "mouse pointer" message.
/// @param action The action of the mouse pointer. Must be #DX_MOUSE_POINTER_ACTION_MOVED, DX_MOUSE_POINTER_ACTION_ENTERED, or DX_MOUSE_POINTER_ACTION_EXITED.
/// @param modifiers The modifiers.
/// @param x,y The position of the mouse pointer.
/// The origin is at the left/top of the canvas. The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @return The zero value on success. A non-zero value on failure.
int dx_mouse_pointer_msg_construct(dx_mouse_pointer_msg* self, uint8_t action, uint8_t modifiers, dx_f32 x, dx_f32 y);

void dx_mouse_pointer_msg_destruct(dx_mouse_pointer_msg* self);

uint8_t dx_mouse_pointer_msg_get_action(dx_mouse_pointer_msg* self);

/// @brief Create a "mouse pointer" message.
/// @param action The action of the mouse pointer. Must be #DX_MOUSE_POINTER_ACTION_MOVED, DX_MOUSE_POINTER_ACTION_ENTERED, or DX_MOUSE_POINTER_ACTION_EXITED.
/// @param modifiers The modifiers.
/// @param x,y The position of the mouse pointer.
/// The origin is at the left/top of the canvas. The positive x-axis is pointing right, the positive y-axis is pointing down.
/// @return A pointer to the message on success. The null pointer on failure.
dx_mouse_pointer_msg* dx_mouse_pointer_msg_create(uint8_t action, uint8_t modifiers, dx_f32 x, dx_f32 y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_HAPTICALS_H_INCLUDED
