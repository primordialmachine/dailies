#include "dx/val/command.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_command_construct_clear_color(dx_command* command, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, DX_VEC4 const* color) {
  command->kind = DX_COMMAND_KIND_CLEAR_COLOR;
  command->clear_color_command.rectangle.l = l;
  command->clear_color_command.rectangle.b = b;
  command->clear_color_command.rectangle.w = w;
  command->clear_color_command.rectangle.h = h;
  command->clear_color_command.color = *color;
  return 0;
}

dx_command* dx_command_create_clear_color(dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, DX_VEC4 const* color) {
  dx_command* command = DX_COMMAND(dx_object_alloc(sizeof(dx_command)));
  if (!command) {
    return NULL;
  }
  if (dx_command_construct_clear_color(command, l, b, w, h, color)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return command;
  }
  return command;
}

int dx_command_construct_clear_depth(dx_command* command, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, dx_f32 depth) {
  command->kind = DX_COMMAND_KIND_CLEAR_DEPTH;
  command->clear_depth_command.rectangle.l = l;
  command->clear_depth_command.rectangle.b = b;
  command->clear_depth_command.rectangle.w = w;
  command->clear_depth_command.rectangle.h = h;
  command->clear_depth_command.depth = depth;
  return 0;
}

dx_command* dx_command_create_clear_depth(dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, dx_f32 depth) {
  dx_command* command = DX_COMMAND(dx_object_alloc(sizeof(dx_command)));
  if (!command) {
    return NULL;
  }
  if (dx_command_construct_clear_depth(command, l, b, w, h, depth)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return command;
  }
  return command;
}

int dx_command_construct_draw(dx_command* command, dx_vbinding* vbinding, dx_val_material* material, dx_cbinding* cbinding, dx_program* program, int start, int length) {
  command->kind = DX_COMMAND_KIND_DRAW;
  command->draw_command.vbinding = vbinding;
  DX_REFERENCE(vbinding);
  command->draw_command.material = material;
  DX_REFERENCE(material);
  command->draw_command.cbinding = cbinding;
  DX_REFERENCE(cbinding);
  command->draw_command.program = program;
  DX_REFERENCE(program);
  command->draw_command.start = start;
  command->draw_command.length = length;
  return 0;
}

dx_command* dx_command_create_draw(dx_vbinding* vbinding, dx_val_material* material, dx_cbinding* cbinding, dx_program* program, int start, int length) {
  dx_command* command = DX_COMMAND(dx_object_alloc(sizeof(dx_command)));
  if (!command) {
    return NULL;
  }
  if (dx_command_construct_draw(command, vbinding, material, cbinding, program, start, length)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return command;
  }
  return command;
}

int dx_command_construct_viewport(dx_command* command, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h) {
  command->kind = DX_COMMAND_KIND_VIEWPORT;
  command->viewport_command.l = l;
  command->viewport_command.b = b;
  command->viewport_command.w = w;
  command->viewport_command.h = h;
  return 0;
}

dx_command* dx_command_create_viewport(dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h) {
  dx_command* command = DX_COMMAND(dx_object_alloc(sizeof(dx_command)));
  if (!command) {
    return NULL;
  }
  if (dx_command_construct_viewport(command, l, b, w, h)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return command;
  }
  return command;
}

static void dx_command_destruct(dx_command* command) {
  switch (command->kind) {
    case DX_COMMAND_KIND_DRAW: {
      DX_UNREFERENCE(command->draw_command.program);
      command->draw_command.program = NULL;
      DX_UNREFERENCE(command->draw_command.cbinding);
      command->draw_command.cbinding = NULL;
      DX_UNREFERENCE(command->draw_command.material);
      command->draw_command.material = NULL;
      DX_UNREFERENCE(command->draw_command.vbinding);
      command->draw_command.vbinding = NULL;
    } break;
  };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.command_list",
                      dx_command_list,
                      dx_object)

int dx_command_list_construct(dx_command_list* command_list) {
  dx_rti_type* type = dx_command_list_get_type();
  if (!type) {
    return 1;
  }
  command_list->size = 0;
  command_list->capacity = 32;
  command_list->elements = dx_memory_allocate(sizeof(dx_command*) * command_list->capacity);
  if (!command_list->elements) {
    dx_log("allocation failed", sizeof("allocation failed"));
    return 1;
  }
  return 0;
}

static void dx_command_list_destruct(dx_command_list* command_list) {
  while (command_list->size > 0) {
    dx_command* element = command_list->elements[--command_list->size];
    DX_UNREFERENCE(element);
    element = NULL;
  }
  dx_memory_deallocate(command_list->elements);
  command_list->elements = NULL;
}

dx_command_list* dx_command_list_create() {
  dx_rti_type* type = dx_command_list_get_type();
  if (!type) {
    return NULL;
  }
  dx_command_list* command_list = DV_COMMAND_LIST(dx_object_alloc(sizeof(dx_command_list)));
  if (!command_list) {
    return NULL;
  }
  if (dx_command_list_construct(command_list)) {
    DX_UNREFERENCE(command_list);
    command_list = NULL;
    return NULL;
  }
  return command_list;
}

// let x = old capacity + additional capacity
// if x overflows, then this function fails.
// otherwise compute new capacity = max(x,y) where
// y = 2 * old capacity if old capacity > 0 and 8 otherwise
// unless y overflows then y = maximal capacity.
static int dx_compute_new_allocated_capacity(dx_size old_capacity, dx_size additional_capacity, dx_size maximal_capacity, dx_size* new_capacity) {
  if (old_capacity > maximal_capacity) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (maximal_capacity - old_capacity < additional_capacity) {
    // soft overflow.
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  dx_size minimal_new_capacity = old_capacity + additional_capacity;
  dx_size best_new_capacity = 0;

  if (old_capacity != 0) {
    if (dx_mul_sz(old_capacity, 2, &best_new_capacity)) {
      // hard overflow. saturate.
      best_new_capacity = maximal_capacity;
    }
  } else {
    best_new_capacity = 8;
  }
  if (best_new_capacity > maximal_capacity) {
    best_new_capacity = maximal_capacity;
  }
  if (minimal_new_capacity > best_new_capacity) {
    best_new_capacity = minimal_new_capacity;
  }
  *new_capacity = best_new_capacity;
  return 0;
}

int dx_command_list_append(dx_command_list* self, dx_command* command) {
  if (self->size == self->capacity) {
    dx_size new_capacity = 0;
    if (dx_compute_new_allocated_capacity(self->capacity, 1, SIZE_MAX / sizeof(dx_command*), &new_capacity)) {
      return 1;
    }
    dx_command** new_elements = dx_memory_reallocate(self->elements, sizeof(dx_command*) * new_capacity);
    if (!new_elements) {
      return 1;
    }
    self->capacity = new_capacity;
    self->elements = new_elements;
  }
  self->elements[self->size] = command;
  self->size++;
  DX_REFERENCE(command);
  return 0;
}

dx_size dx_command_list_get_size(dx_command_list const* self) {
  return self->size;
}

dx_command* dx_command_list_get_at(dx_command_list const* self, dx_size index) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  if (index >= self->size) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  return self->elements[index];
}
