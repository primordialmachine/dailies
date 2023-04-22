#include "dx/program_text.h"

// free
#include <malloc.h>

int dx_program_text_construct_from_file(dx_program_text* program_text, dx_program_text_type type, dx_string* path) {
  if (dx_string_contains_symbol(path, '\0')) {
    return 1;
  }
  dx_string* format = dx_string_create("%s\0", sizeof("%s\0"));
  if (!format) {
    return 1;
  }
  dx_string *path1 = dx_string_printf(format, path);
  DX_UNREFERENCE(format);
  if (!path1) {
    return 1;
  }

  char* bytes; size_t number_of_bytes;
  if (dx_get_file_contents(path1->bytes, &bytes, &number_of_bytes)) {
    DX_UNREFERENCE(path1);
    path1 = NULL;
    return 1;
  }
  DX_UNREFERENCE(path1);
  path1 = NULL;
  program_text->program_text = dx_string_create(bytes, number_of_bytes);
  free(bytes);
  if (!program_text->program_text) {
    return 1;
  }
  program_text->parent = NULL;
  program_text->type = type;
  DX_OBJECT(program_text)->destruct = (void(*)(dx_object*)) & dx_program_text_destruct;
  return 0;
}

int dx_program_text_construct(dx_program_text* program_text, dx_program_text* vertex_program_text, dx_program_text* fragment_program_text) {
  if (!vertex_program_text || DX_PROGRAM_TEXT_TYPE_VERTEX != vertex_program_text->type) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (!fragment_program_text || DX_PROGRAM_TEXT_TYPE_FRAGMENT != fragment_program_text->type) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  
  program_text->vertex_program_text = vertex_program_text;
  DX_REFERENCE(vertex_program_text);
  program_text->vertex_program_text->parent = program_text;

  program_text->fragment_program_text = fragment_program_text;
  DX_REFERENCE(fragment_program_text);
  program_text->fragment_program_text->parent = program_text;
  
  program_text->type = DX_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT;
  
  DX_OBJECT(program_text)->destruct = (void(*)(dx_object*)) & dx_program_text_destruct;
  return 0;
}

void dx_program_text_destruct(dx_program_text* program_text) {
  switch (program_text->type) {
  case DX_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT: {
    DX_UNREFERENCE(program_text->vertex_program_text);
    program_text->vertex_program_text = NULL;
    DX_UNREFERENCE(program_text->fragment_program_text);
    program_text->fragment_program_text = NULL;
  } break;
  case DX_PROGRAM_TEXT_TYPE_VERTEX:
  case DX_PROGRAM_TEXT_TYPE_FRAGMENT: {
    DX_UNREFERENCE(program_text->program_text);
    program_text->program_text = NULL;
  } break;
  };
}

dx_program_text* dx_program_text_create_from_file(dx_program_text_type type, dx_string* path) {
  dx_program_text* program_text = DX_PROGRAM_TEXT(dx_object_alloc(sizeof(dx_program_text)));
  if (!program_text) {
    return NULL;
  }
  if (dx_program_text_construct_from_file(program_text, type, path)) {
    DX_UNREFERENCE(program_text);
    program_text = NULL;
    return NULL;
  }
  return program_text;
}


dx_program_text* dx_program_text_create(dx_program_text *vertex_program_text, dx_program_text* fragment_program_text) {
  dx_program_text* program_text = DX_PROGRAM_TEXT(dx_object_alloc(sizeof(dx_program_text)));
  if (!program_text) {
    return NULL;
  }
  if (dx_program_text_construct(program_text, vertex_program_text, fragment_program_text)) {
    DX_UNREFERENCE(program_text);
    program_text = NULL;
    return NULL;
  }
  return program_text;  
}

static int add_define(dx_program_text* self, dx_string* name) {
  dx_string* format = dx_string_create("#define %s (1)\n%s", sizeof("#define %s (1)\n%s"));
  if (!format) {
    return 1;
  }
  dx_string* program_text = dx_string_printf(format, name, self->program_text);
  DX_UNREFERENCE(format);
  format = NULL;
  if (!program_text) {
    return 1;
  }
  DX_UNREFERENCE(self->program_text);
  self->program_text = NULL;
  self->program_text = program_text;
  return 0;
}

int dx_program_text_add_define(dx_program_text* self, dx_string* name) {
  switch (self->type) {
  case DX_PROGRAM_TEXT_TYPE_FRAGMENT: {
    add_define(self, name);
    dx_program_text* p = self->parent;
    if (p) {
      p = p->vertex_program_text;
      add_define(p, name);
    }
    return 0;
  } break;
  case DX_PROGRAM_TEXT_TYPE_VERTEX: {
    add_define(self, name);
    dx_program_text* p = self->parent;
    if (p) {
      p = p->fragment_program_text;
      add_define(p, name);
    }
    return 0;
  } break;
  case DX_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT: {
    add_define(self->vertex_program_text, name);
    add_define(self->fragment_program_text, name);
    return 0;//return dx_program_text_add_define(self->vertex_program_text, name);
  } break;
  }
  return 0;
}
