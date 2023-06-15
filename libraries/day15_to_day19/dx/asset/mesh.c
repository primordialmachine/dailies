#include "dx/asset/mesh.h"

#include "dx/asset/mesh/generators.h"

// malloc, realloc, free
#include <malloc.h>
// memcpy
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.mesh", dx_asset_mesh, dx_object)

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

  DX_VEC3* xyz = malloc(number_of_vertices * sizeof(DX_VEC3) != 0 ? number_of_vertices * sizeof(DX_VEC3) : 1);
  if (!xyz) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  memcpy(xyz, self->vertices.xyz, dx_min_sz(self->number_of_vertices, number_of_vertices) * sizeof(DX_VEC3));

  DX_VEC4* ambient_rgba = malloc(number_of_vertices * sizeof(DX_VEC4) != 0 ? number_of_vertices * sizeof(DX_VEC4) : 1);
  if (!ambient_rgba) {
    free(xyz);
    xyz = NULL;
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  memcpy(ambient_rgba, self->vertices.ambient_rgba, dx_min_sz(self->number_of_vertices, number_of_vertices) * sizeof(DX_VEC4));

  DX_VEC2* ambient_uv = malloc(number_of_vertices * sizeof(DX_VEC2) != 0 ? number_of_vertices * sizeof(DX_VEC2) : 1);
  if (!ambient_uv) {
    free(xyz);
    xyz = NULL;
    free(ambient_rgba);
    ambient_rgba = NULL;
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  memcpy(ambient_uv, self->vertices.ambient_uv, dx_min_sz(self->number_of_vertices, number_of_vertices) * sizeof(DX_VEC2));

  self->vertices.xyz = xyz;
  self->vertices.ambient_rgba = ambient_rgba;
  self->vertices.ambient_uv = ambient_uv;
  self->number_of_vertices = number_of_vertices;

  return 0;
}

static void dx_asset_mesh_destruct(dx_asset_mesh* self) {
  if (self->material) {
    DX_UNREFERENCE(self->material);
    self->material = NULL;
  }
  if (self->vertices.ambient_uv) {
    free(self->vertices.ambient_uv);
    self->vertices.ambient_uv = NULL;
  }
  if (self->vertices.ambient_rgba) {
    free(self->vertices.ambient_rgba);
    self->vertices.ambient_rgba = NULL;
  }
  if (self->vertices.xyz) {
    free(self->vertices.xyz);
    self->vertices.xyz = NULL;
  }
}

static int dx_asset_mesh_construct(dx_asset_mesh* self, dx_string* specifier, dx_asset_material* material) {
  if (!self || !specifier || !material) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  // "default" mesh
  self->vertices.xyz = malloc(1);
  if (!self->vertices.xyz) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }

  self->vertices.ambient_rgba = malloc(1);
  if (!self->vertices.ambient_rgba) {
    dx_set_error(DX_ALLOCATION_FAILED);
    free(self->vertices.xyz);
    self->vertices.xyz = NULL;
    return 1;
  }

  self->vertices.ambient_uv = malloc(1);
  if (!self->vertices.ambient_uv) {
    dx_set_error(DX_ALLOCATION_FAILED);
    free(self->vertices.ambient_rgba);
    self->vertices.ambient_rgba = NULL;
    free(self->vertices.xyz);
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

SELECT_GENERATOR(cube)
SELECT_GENERATOR(empty)
SELECT_GENERATOR(quadriliteral)
SELECT_GENERATOR(triangle)
SELECT_GENERATOR(octahedron)

#undef SELECT_GENERATOR
  
  if (!generator) {
    dx_set_error(DX_INVALID_ARGUMENT);
    free(self->vertices.ambient_uv);
    self->vertices.ambient_uv = NULL;
    free(self->vertices.ambient_rgba);
    self->vertices.ambient_rgba = NULL;
    free(self->vertices.xyz);
    self->vertices.xyz = NULL;
    return 1;
  }

  if ((*generator)(self)) {
    free(self->vertices.ambient_uv);
    self->vertices.ambient_uv = NULL;
    free(self->vertices.ambient_rgba);
    self->vertices.ambient_rgba = NULL;
    free(self->vertices.xyz);
    self->vertices.xyz = NULL;
    return 1;
  }
  self->material = material;
  DX_REFERENCE(material);
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_mesh_destruct;
  return 0;
}

dx_asset_mesh* dx_asset_mesh_create(dx_string* specifier, dx_asset_material* material) {
  dx_asset_mesh* self = DX_ASSET_MESH(dx_object_alloc(sizeof(dx_asset_mesh)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_mesh_construct(self, specifier, material)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_asset_mesh_format(dx_asset_mesh* self, DX_VERTEX_FORMAT vertex_format, void** bytes, size_t* number_of_bytes) {
  switch (vertex_format) {
  case DX_VERTEX_FORMAT_POSITION: {
    void* p = malloc(self->number_of_vertices * sizeof(DX_VEC3));
    if (!p) {
      dx_set_error(DX_ALLOCATION_FAILED);
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
  case DX_VERTEX_FORMAT_COLOR: {
    void* p = malloc(self->number_of_vertices * sizeof(DX_VEC4));
    if (!p) {
      dx_set_error(DX_ALLOCATION_FAILED);
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
  case DX_VERTEX_FORMAT_POSITION_COLOR: {
    void* p = malloc(self->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC4)));
    if (!p) {
      dx_set_error(DX_ALLOCATION_FAILED);
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
  case DX_VERTEX_FORMAT_POSITION_TEXTURE: {
    void* p = malloc(self->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC2)));
    if (!p) {
      dx_set_error(DX_ALLOCATION_FAILED);
      return 1;
    }
    char* q = (char*)p;
    for (size_t i = 0, n = self->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = self->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((DX_VEC2*)q) = self->vertices.ambient_uv[i];
      q += sizeof(DX_VEC2);
    }
    *bytes = p;
    *number_of_bytes = self->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC2));
    return 0;
  } break;
  default: {
    return 1;
  } break;
  };
  return 0;
}

int dx_asset_mesh_transform_range(dx_asset_mesh* self, DX_MAT4 const* a, size_t i, size_t n) {
  if (!self || !a) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (i + n > self->number_of_vertices) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  for (size_t j = i, m = i + n; j < m; ++j) {
    dx_transform_point(&self->vertices.xyz[j], &self->vertices.xyz[j], a);
  }
  return 0;
}

int dx_asset_mesh_append_quadriliteral(dx_asset_mesh* self) {
  static const float a = -0.5f;
  static const float b = +0.5f;

  DX_VEC3 p[4];
  DX_VEC2 t[4];
  DX_VEC4 c[4];

  p[0] = (DX_VEC3){ a, a, 0.f }; // left, bottom
  t[0] = (DX_VEC2){ 0.f, 0.f };
  c[0] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  p[1] = (DX_VEC3){ b, a, 0.f }; // right, bottom
  t[1] = (DX_VEC2){ 1.f, 0.f };
  c[1] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  p[2] = (DX_VEC3){ b, b, 0.f }; // right, top
  t[2] = (DX_VEC2){ 1.f, 1.f };
  c[2] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  p[3] = (DX_VEC3){ a, b, 0.f }; // left, top
  t[3] = (DX_VEC2){ 0.f, 1.f };
  c[3] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };

  size_t i = self->number_of_vertices;
  if (resize_vertex_arrays(self, false, i + 6)) {
    return 1;
  }
  
  // triangle #1

  // left top
  self->vertices.xyz[i] = p[3];
  self->vertices.ambient_rgba[i] = c[3];
  self->vertices.ambient_uv[i] = t[3];
  i++;

  // left bottom
  self->vertices.xyz[i] = p[0];
  self->vertices.ambient_rgba[i] = c[0];
  self->vertices.ambient_uv[i] = t[0];
  i++;

  // right top
  self->vertices.xyz[i] = p[2];
  self->vertices.ambient_rgba[i] = c[2];
  self->vertices.ambient_uv[i] = t[2];
  i++;

  // triangle #2

  // right top
  self->vertices.xyz[i] = p[2];
  self->vertices.ambient_rgba[i] = c[2];
  self->vertices.ambient_uv[i] = t[2];
  i++;

  // left bottom
  self->vertices.xyz[i] = p[0];
  self->vertices.ambient_rgba[i] = c[0];
  self->vertices.ambient_uv[i] = t[0];
  i++;

  // right bottom
  self->vertices.xyz[i] = p[1];
  self->vertices.ambient_rgba[i] = c[1];
  self->vertices.ambient_uv[i] = t[1];
  i++;

  return 0;
}

int dx_asset_mesh_append_vertex(dx_asset_mesh* self,
                                DX_VEC3 const* xyz,
                                DX_VEC4 const* ambient_rgba,
                                DX_VEC2 const* ambient_uv)
{
  size_t i = self->number_of_vertices;
  if (resize_vertex_arrays(self, false, i + 1)) {
    return 1;
  }
  self->vertices.xyz[i] = *xyz;
  self->vertices.ambient_rgba[i] = *ambient_rgba;
  self->vertices.ambient_uv[i] = *ambient_uv;
  return 0;
}

int dx_asset_mesh_clear(dx_asset_mesh* self) {
  return resize_vertex_arrays(self, true, 0);
}

void dx_asset_mesh_set_mesh_ambient_rgba(dx_asset_mesh* self, DX_VEC4 const* value) {
  self->mesh.ambient_rgba = *value;
}

int dx_asset_mesh_append_range(dx_asset_mesh* self, size_t i, size_t n) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (i + n > self->number_of_vertices) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  size_t j = self->number_of_vertices;
  if (resize_vertex_arrays(self, false, j + n)) {
    return 1;
  }
  for (size_t k = 0; k < n; ++k) {
    self->vertices.xyz[j + k] = self->vertices.xyz[i + k];
    self->vertices.ambient_rgba[j + k] = self->vertices.ambient_rgba[i + k];
    self->vertices.ambient_uv[j + k] = self->vertices.ambient_uv[i + k];
  }
  return 0;
}
