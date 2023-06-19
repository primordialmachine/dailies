#include "dx/asset/mesh.h"

#include "dx/asset/mesh/generators.h"

// memcpy
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline size_t dx_min_sz(size_t x, size_t y) {
  return x < y ? x : y;
}

static int resize_vertex_arrays(dx_asset_mesh* self, bool shrink, size_t number_of_vertices) {
  if (self->number_of_vertices == number_of_vertices) {
    // if the number of vertices in the mesh is equal to the required number of vertices, return success.
    return 0;
  }
  if (self->number_of_vertices > number_of_vertices && !shrink) {
    // if the number of vertices in the mesh is greater than the required and shrinking is not desired, return success.
    return 0;
  }
  // two cases:
  // - the number of vertices in the mesh is smaller than the required number of vertices or
  // - the number of vertices in the mesh is greater than the deisred number of vertices and shrinking is desired

  DX_VEC3* xyz = dx_memory_allocate(number_of_vertices * sizeof(DX_VEC3));
  if (!xyz) {
    return 1;
  }
  dx_memory_copy(xyz, self->vertices.xyz, dx_min_sz(self->number_of_vertices, number_of_vertices) * sizeof(DX_VEC3));

  DX_VEC4* ambient_rgba = dx_memory_allocate(number_of_vertices * sizeof(DX_VEC4));
  if (!ambient_rgba) {
    dx_memory_deallocate(xyz);
    xyz = NULL;
    return 1;
  }
  dx_memory_copy(ambient_rgba, self->vertices.ambient_rgba, dx_min_sz(self->number_of_vertices, number_of_vertices) * sizeof(DX_VEC4));

  self->vertices.xyz = xyz;
  self->vertices.ambient_rgba = ambient_rgba;
  self->number_of_vertices = number_of_vertices;

  return 0;
}

static void dx_asset_mesh_destruct(dx_asset_mesh* self) {
  if (self->vertices.ambient_rgba) {
    dx_memory_deallocate(self->vertices.ambient_rgba);
    self->vertices.ambient_rgba = NULL;
  }
  if (self->vertices.xyz) {
    dx_memory_deallocate(self->vertices.xyz);
    self->vertices.xyz = NULL;
  }
}

static int dx_asset_mesh_construct(dx_asset_mesh* self, dx_string* specifier) {
  if (!self || !specifier) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  // "default" mesh
  self->vertices.xyz = dx_memory_allocate(0);
  if (!self->vertices.xyz) {
    return 1;
  }

  self->vertices.ambient_rgba = dx_memory_allocate(0);
  if (!self->vertices.ambient_rgba) {
    dx_memory_deallocate(self->vertices.xyz);
    self->vertices.xyz = NULL;
    return 1;
  }

  self->mesh.ambient_rgba = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  self->number_of_vertices = 0;
  
  int (*generator)(dx_asset_mesh*)  = NULL;

#define SELECT_GENERATOR(name) \
  { \
    dx_string* temporary = dx_string_create(#name, strlen(#name)); \
    if (dx_string_is_equal_to(specifier, temporary)) { \
      generator = &dx_asset_mesh_on_##name; \
    } \
  }

SELECT_GENERATOR(empty)
SELECT_GENERATOR(quadriliteral)
SELECT_GENERATOR(triangle)

#undef SELECT_GENERATOR
  
  if (!generator) {
    dx_set_error(DX_INVALID_ARGUMENT);
    dx_memory_deallocate(self->vertices.ambient_rgba);
    self->vertices.ambient_rgba = NULL;
    dx_memory_deallocate(self->vertices.xyz);
    self->vertices.xyz = NULL;
    return 1;
  }

  if ((*generator)(self)) {
    dx_memory_deallocate(self->vertices.ambient_rgba);
    self->vertices.ambient_rgba = NULL;
    dx_memory_deallocate(self->vertices.xyz);
    self->vertices.xyz = NULL;
    return 1;
  }
  self->vertex_format = DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA_AMBIENT_UV;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_mesh_destruct;
  return 0;
}

dx_asset_mesh* dx_asset_mesh_create(dx_string* specifier) {
  dx_asset_mesh* self = DX_ASSET_MESH(dx_object_alloc(sizeof(dx_asset_mesh)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_mesh_construct(self, specifier)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_asset_mesh_format(dx_asset_mesh* self, DX_VERTEX_FORMAT vertex_format, void** bytes, size_t* number_of_bytes) {
  switch (vertex_format) {
  case DX_VERTEX_FORMAT_POSITION_XYZ: {
    void* p = dx_memory_allocate(self->number_of_vertices * sizeof(DX_VEC3));
    if (!p) {
      return 1;
    }
    char* q = (char*)p;
    for (size_t i = 0, n = self->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = self->vertices.xyz[i];
      q += sizeof(DX_VEC3);
    }
    *bytes = p;
    *number_of_bytes = self->number_of_vertices * (sizeof(DX_VEC3));
    return 0;
  } break;
  case DX_VERTEX_FORMAT_AMBIENT_RGBA: {
    void* p = dx_memory_allocate(self->number_of_vertices * sizeof(DX_VEC4));
    if (!p) {
      return 1;
    }
    char* q = (char*)p;
    for (size_t i = 0, n = self->number_of_vertices; i < n; ++i) {
      *((DX_VEC4*)q) = self->vertices.ambient_rgba[i];
      q += sizeof(DX_VEC4);
    }
    *bytes = p;
    *number_of_bytes = self->number_of_vertices * (sizeof(DX_VEC4));
    return 0;
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA: {
    void* p = dx_memory_allocate(self->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC4)));
    if (!p) {
      return 1;
    }
    char* q = (char*)p;
    for (size_t i = 0, n = self->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = self->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((DX_VEC4*)q) = self->vertices.ambient_rgba[i];
      q += sizeof(DX_VEC4);
    }
    *bytes = p;
    *number_of_bytes = self->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC4));
    return 0;
  } break;
  default: {
    return 1;
  } break;
  };
  return 0;
}

int dx_asset_mesh_append_quadriliteral(dx_asset_mesh* self) {
  static const float a = -0.5f;
  static const float b = +0.5f;

  DX_VEC3 p[4];
  DX_VEC4 c[4];

  p[0] = (DX_VEC3){ a, a, 0.f }; // left, bottom
  c[0] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  p[1] = (DX_VEC3){ b, a, 0.f }; // right, bottom
  c[1] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  p[2] = (DX_VEC3){ b, b, 0.f }; // right, top
  c[2] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  p[3] = (DX_VEC3){ a, b, 0.f }; // left, top
  c[3] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };

  size_t i = self->number_of_vertices;
  if (resize_vertex_arrays(self, false, i + 6)) {
    return 1;
  }
  
  // triangle #1

  // left top
  self->vertices.xyz[i] = p[3];
  self->vertices.ambient_rgba[i] = c[3];
  i++;

  // left bottom
  self->vertices.xyz[i] = p[0];
  self->vertices.ambient_rgba[i] = c[0];
  i++;

  // right top
  self->vertices.xyz[i] = p[2];
  self->vertices.ambient_rgba[i] = c[2];
  i++;

  // triangle #2

  // right top
  self->vertices.xyz[i] = p[2];
  self->vertices.ambient_rgba[i] = c[2];
  i++;

  // left bottom
  self->vertices.xyz[i] = p[0];
  self->vertices.ambient_rgba[i] = c[0];
  i++;

  // right bottom
  self->vertices.xyz[i] = p[1];
  self->vertices.ambient_rgba[i] = c[1];
  i++;

  return 0;
}

int dx_asset_mesh_append_vertex(dx_asset_mesh* self,
                                DX_VEC3 const* xyz,
                                DX_VEC4 const* ambient_rgba)
{
  size_t i = self->number_of_vertices;
  if (resize_vertex_arrays(self, false, i + 1)) {
    return 1;
  }
  self->vertices.xyz[i] = *xyz;
  self->vertices.ambient_rgba[i] = *ambient_rgba;
  return 0;
}

int dx_asset_mesh_clear(dx_asset_mesh* self) {
  return resize_vertex_arrays(self, true, 0);
}

void dx_asset_mesh_set_mesh_ambient_rgba(dx_asset_mesh* self, DX_VEC4 const* value) {
  self->mesh.ambient_rgba = *value;
}
