#include "dx/val/mesh.h"

// malloc, free
#include <malloc.h>
// snprintf
#include <stdio.h>
#include "dx/val/program_text.h"

/// @brief See shader program source code for details.
#define DX_PROGRAM_WITH_MESH_AMBIENT_RGBA (1)

/// @brief See shader program source code for details.
#define DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA (2)

/// @brief See shader program source code for details.
#define DX_PROGRAM_WITH_VERTEX_AMBIENT_UV (4)

/// @brief See shader program source code for details.
#define DX_PROGRAM_WITH_AMBIENT_TEXTURE (8)

// @brief
// Load a program consisting of a vertex and a fragment shader.
// @param path
// The path (relative or absolute) to the directory where the two shader programs reside.
// @param filename
// The filename (without path or extension) of the shader programs.
// @param flags
// Flags to customize the behavior of the shaders are load time.
// @remarks
// This function loads the vertex shader @code{<path>/<filename>.vs} and the fragment shader @code{<path>/<filename>.fs}.
// The shaders can be adjusted at load time via the @a flags.
static dx_program_text* load_program(dx_string* path, dx_string* filename, uint8_t flags);

static int add_material_to_backend(dx_mesh* self);

static void remove_material_from_backend(dx_mesh* self);

static int add_to_backend(dx_mesh* self);

static void remove_from_backend(dx_mesh* self);

static dx_program_text* load_program(dx_string* path, dx_string* filename, uint8_t flags) {
  dx_program_text* vertex_program = NULL, * fragment_program = NULL;
  dx_program_text* program = NULL;
  {
    dx_string* format = dx_string_create("%s/%s.vs", sizeof("%s/%s.vs") - 1);
    if (!format) {
      goto on_error;
    }
    dx_string* p = dx_string_printf(format, path, filename);
    DX_UNREFERENCE(format);
    format = NULL;
    if (!p) {
      goto on_error;
    }
    vertex_program = dx_program_text_create_from_file(DX_PROGRAM_TEXT_TYPE_VERTEX, p);
    DX_UNREFERENCE(p);
    p = NULL;
    if (!vertex_program) {
      goto on_error;
    }
  }
  {
    dx_string* format = dx_string_create("%s/%s.fs", sizeof("%s/%s.fs") - 1);
    if (!format) {
      goto on_error;
    }
    dx_string* p = dx_string_printf(format, path, filename);
    DX_UNREFERENCE(format);
    format = NULL;
    if (!p) {
      goto on_error;
    }
    fragment_program = dx_program_text_create_from_file(DX_PROGRAM_TEXT_TYPE_FRAGMENT, p);
    DX_UNREFERENCE(p);
    p = NULL;
    if (!fragment_program) {
      goto on_error;
    }
  }
  {
    program = dx_program_text_create(vertex_program, fragment_program);
    DX_UNREFERENCE(vertex_program);
    vertex_program = NULL;
    DX_UNREFERENCE(fragment_program);
    fragment_program = NULL;
    if (!program) {
      goto on_error;
    }
    if (DX_PROGRAM_WITH_MESH_AMBIENT_RGBA == (flags & DX_PROGRAM_WITH_MESH_AMBIENT_RGBA)) {
      dx_string* name = dx_string_create("WITH_MESH_AMBIENT_RGBA",
        sizeof("WITH_MESH_AMBIENT_RGBA") - 1);
      if (!name) {
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_program_text_add_define(program, name)) {
        DX_UNREFERENCE(name);
        name = NULL;
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      DX_UNREFERENCE(name);
      name = NULL;
    }
    if (DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA == (flags & DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA)) {
      dx_string* name = dx_string_create("WITH_VERTEX_AMBIENT_RGBA",
        sizeof("WITH_VERTEX_AMBIENT_RGBA") - 1);
      if (!name) {
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_program_text_add_define(program, name)) {
        DX_UNREFERENCE(name);
        name = NULL;
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      DX_UNREFERENCE(name);
      name = NULL;
    }
    if (DX_PROGRAM_WITH_VERTEX_AMBIENT_UV == (flags & DX_PROGRAM_WITH_VERTEX_AMBIENT_UV)) {
      dx_string* name = dx_string_create("WITH_VERTEX_AMBIENT_UV",
        sizeof("WITH_VERTEX_AMBIENT_UV") - 1);
      if (!name) {
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_program_text_add_define(program, name)) {
        DX_UNREFERENCE(name);
        name = NULL;
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      DX_UNREFERENCE(name);
      name = NULL;
    }
    if (DX_PROGRAM_WITH_AMBIENT_TEXTURE == (flags & DX_PROGRAM_WITH_AMBIENT_TEXTURE)) {
      dx_string* name = dx_string_create("WITH_AMBIENT_TEXTURE",
        sizeof("WITH_AMBIENT_TEXTURE") - 1);
      if (!name) {
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_program_text_add_define(program, name)) {
        DX_UNREFERENCE(name);
        name = NULL;
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      DX_UNREFERENCE(name);
      name = NULL;
    }
  }
  return program;
on_error:
  if (program) {
    DX_UNREFERENCE(program);
    program = NULL;
  }
  if (fragment_program) {
    DX_UNREFERENCE(fragment_program);
    fragment_program = NULL;
  }
  if (vertex_program) {
    DX_UNREFERENCE(vertex_program);
    vertex_program = NULL;
  }
  return NULL;
}

static int add_material_to_backend(dx_mesh* self) {
  self->material = dx_material_create(self->context, self->asset_mesh->material);
  if (!self->material) {
    return 1;
  }
  return 0;
}

static void remove_material_from_backend(dx_mesh* self) {
  if (self->material) {
    DX_UNREFERENCE(self->material);
    self->material = NULL;
  }
}

static int add_to_backend(dx_mesh* self) {
  DX_VERTEX_FORMAT vertex_format = self->asset_mesh->vertex_format;

  // create buffer
  self->buffer = dx_context_create_buffer(self->context);
  if (!self->buffer) {
    return 1;
  }

  // upload data to buffer
  void* bytes; size_t number_of_bytes;
  if (dx_asset_mesh_format(self->asset_mesh, vertex_format, &bytes, &number_of_bytes)) {
    return 1;
  }
  if (dx_buffer_set_data(self->buffer, bytes, number_of_bytes)) {
    free(bytes);
    bytes = NULL;
    return 1;
  }
  free(bytes);
  bytes = NULL;

  // create variable binding
  self->vbinding = dx_context_create_vbinding(self->context, vertex_format, self->buffer);
  if (!self->vbinding) {
    DX_UNREFERENCE(self->buffer);
    self->buffer = NULL;
    return 1;
  }

  // create the program
  uint8_t flags = DX_PROGRAM_WITH_MESH_AMBIENT_RGBA;
  switch (vertex_format) {
  case DX_VERTEX_FORMAT_POSITION_XYZ: {
    /*Intentionally empty.*/
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA: {
    flags |= DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA;
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_UV: {
   flags |= DX_PROGRAM_WITH_VERTEX_AMBIENT_UV;
   if (self->material->asset_material->ambient_texture) {
      flags |= DX_PROGRAM_WITH_AMBIENT_TEXTURE;
    }
  } break;
  default: {
    DX_UNREFERENCE(self->vbinding);
    self->vbinding = NULL;
    DX_UNREFERENCE(self->buffer);
    self->buffer = NULL;
    return 1;
  } break;
  };
  dx_string* path = dx_string_create("assets/gl", sizeof("assets/gl") - 1);
  if (!path) {
    return 1;
  }
  dx_string* filename = dx_string_create("3", sizeof("3") - 1);
  if (!filename) {
    DX_UNREFERENCE(path);
    path = NULL;
    return 1;
  }
  dx_program_text* program_text = load_program(path, filename, flags);
  DX_UNREFERENCE(filename);
  filename = NULL;
  DX_UNREFERENCE(path);
  path = NULL;
  if (!program_text) {
    DX_UNREFERENCE(self->vbinding);
    self->vbinding = NULL;
    DX_UNREFERENCE(self->buffer);
    self->buffer = NULL;
    return 1;
  }
  self->program = dx_context_create_program(self->context, program_text);
  DX_UNREFERENCE(program_text);
  program_text = NULL;
  if (!self->program) {
    DX_UNREFERENCE(self->vbinding);
    self->vbinding = NULL;
    DX_UNREFERENCE(self->buffer);
    self->buffer = NULL;
    return 1;
  }

  return 0;
}

static void remove_from_backend(dx_mesh* self) {
  if (self->program) {
    DX_UNREFERENCE(self->program);
    self->program = NULL;
  }
  if (self->vbinding) {
    DX_UNREFERENCE(self->vbinding);
    self->vbinding = NULL;
  }
  if (self->buffer) {
    DX_UNREFERENCE(self->buffer);
    self->buffer = NULL;
  }
}

int dx_mesh_construct(dx_mesh* self, dx_context* context, dx_asset_mesh* asset_mesh) {
  self->asset_mesh = asset_mesh;
  DX_REFERENCE(asset_mesh);

  self->context = context;

  if (add_material_to_backend(self)) {
    DX_UNREFERENCE(self->asset_mesh);
    self->asset_mesh = NULL;
  }

  if (add_to_backend(self)) {
    remove_material_from_backend(self);
    DX_UNREFERENCE(self->asset_mesh);
    self->asset_mesh = NULL;
    return 1;
  }

  DX_OBJECT(self)->destruct = (void (*) (dx_object*)) & dx_mesh_destruct;
  return 0;
}

void dx_mesh_destruct(dx_mesh* self) {
  remove_material_from_backend(self);
  remove_from_backend(self);
  DX_UNREFERENCE(self->asset_mesh);
  self->asset_mesh = NULL;
}

dx_mesh* dx_mesh_create(dx_context* context, dx_asset_mesh* asset_mesh) {
  dx_mesh* self = DX_MESH(dx_object_alloc(sizeof(dx_mesh)));
  if (!self) {
    return NULL;
  }
  if (dx_mesh_construct(self, context, asset_mesh)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
