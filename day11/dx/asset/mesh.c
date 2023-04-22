#include "dx/asset/mesh.h"

#include "dx/asset/mesh/generators.h"

// malloc, realloc, free
#include <malloc.h>
// memcpy
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline size_t dx_min_sz(size_t x, size_t y) {
  return x < y ? x : y;
}

static int resize_vertex_arrays(dx_asset_mesh* mesh, bool shrink, size_t number_of_vertices) {
  if (mesh->number_of_vertices == number_of_vertices) {
    // if the number of vertices in the mesh is equal to the required number of vertices, return success.
    return 0;
  }
  if (mesh->number_of_vertices > number_of_vertices && !shrink) {
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
  memcpy(xyz, mesh->vertices.xyz, dx_min_sz(mesh->number_of_vertices, number_of_vertices) * sizeof(DX_VEC3));

  DX_VEC4* ambient_rgba = malloc(number_of_vertices * sizeof(DX_VEC4) != 0 ? number_of_vertices * sizeof(DX_VEC4) : 1);
  if (!ambient_rgba) {
    free(xyz);
    xyz = NULL;
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  memcpy(ambient_rgba, mesh->vertices.ambient_rgba, dx_min_sz(mesh->number_of_vertices, number_of_vertices) * sizeof(DX_VEC4));

  mesh->vertices.xyz = xyz;
  mesh->vertices.ambient_rgba = ambient_rgba;
  mesh->number_of_vertices = number_of_vertices;

  return 0;
}

static void dx_asset_mesh_destruct(dx_asset_mesh* mesh) {
  if (mesh->vertices.ambient_rgba) {
    free(mesh->vertices.ambient_rgba);
    mesh->vertices.ambient_rgba = NULL;
  }
  if (mesh->vertices.xyz) {
    free(mesh->vertices.xyz);
    mesh->vertices.xyz = NULL;
  }
}

// create a triangle mesh
static int _triangle(dx_asset_mesh* mesh) {
  static size_t const number_of_vertices = 3;
  static DX_VEC3 const xyz[] = {
    { -0.5f, -0.5f, 0.f, },
    { +0.5f, -0.5f, 0.f, },
    { +0.0f, +0.5f, 0.f, },
  };
  if (resize_vertex_arrays(mesh, true, number_of_vertices)) {
    return 1;
  }
  memcpy(mesh->vertices.xyz, xyz, number_of_vertices * sizeof(DX_VEC3));
  for (size_t i = 0, n = mesh->number_of_vertices; i < n; ++i) {
    static const DX_VEC4 color = { 1.f, 1.f, 1.f, 1.f };
    mesh->vertices.ambient_rgba[i] = color;
  }
  mesh->mesh.ambient_rgba = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  return 0;
}

static int dx_asset_mesh_construct(dx_asset_mesh* mesh, char const* specifier) {
  // "default" mesh
  mesh->vertices.xyz = malloc(1);
  if (!mesh->vertices.xyz) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }

  mesh->vertices.ambient_rgba = malloc(1);
  if (!mesh->vertices.ambient_rgba) {
    dx_set_error(DX_ALLOCATION_FAILED);
    free(mesh->vertices.xyz);
    mesh->vertices.xyz = NULL;
    return 1;
  }

  mesh->mesh.ambient_rgba = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  mesh->number_of_vertices = 0;
  
  int (*generator)(dx_asset_mesh*)  = NULL;
  if (!strcmp(specifier, "empty"))
    generator = &dx_asset_mesh_on_empty;
  else if (!strcmp(specifier, "quadriliteral"))
    generator = &dx_asset_mesh_on_quadriliteral;
  else if (!strcmp(specifier, "triangle"))
    generator = &_triangle;
  else {
    dx_set_error(DX_INVALID_ARGUMENT);
    free(mesh->vertices.ambient_rgba);
    mesh->vertices.ambient_rgba = NULL;
    free(mesh->vertices.xyz);
    mesh->vertices.xyz = NULL;
    return 1;
  }

  if ((*generator)(mesh)) {
    free(mesh->vertices.ambient_rgba);
    mesh->vertices.ambient_rgba = NULL;
    free(mesh->vertices.xyz);
    mesh->vertices.xyz = NULL;
    return 1;
  }
  DX_OBJECT(mesh)->destruct = (void(*)(dx_object*)) & dx_asset_mesh_destruct;
  return 0;
}

dx_asset_mesh* dx_asset_mesh_create(char const* specifier) {
  dx_asset_mesh* mesh = DX_ASSET_MESH(dx_object_alloc(sizeof(dx_asset_mesh)));
  if (!mesh) {
    return NULL;
  }
  if (dx_asset_mesh_construct(mesh, specifier)) {
    DX_UNREFERENCE(mesh);
    mesh = NULL;
    return NULL;
  }
  return mesh;
}

int dx_asset_mesh_append_quadriliteral(dx_asset_mesh* mesh) {
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

  size_t i = mesh->number_of_vertices;
  if (resize_vertex_arrays(mesh, false, i + 6)) {
    return 1;
  }
  
  // triangle #1

  // left top
  mesh->vertices.xyz[i] = p[3];
  mesh->vertices.ambient_rgba[i] = c[3];
  i++;

  // left bottom
  mesh->vertices.xyz[i] = p[0];
  mesh->vertices.ambient_rgba[i] = c[0];
  i++;

  // right top
  mesh->vertices.xyz[i] = p[2];
  mesh->vertices.ambient_rgba[i] = c[2];
  i++;

  // triangle #2

  // right top
  mesh->vertices.xyz[i] = p[2];
  mesh->vertices.ambient_rgba[i] = c[2];
  i++;

  // left bottom
  mesh->vertices.xyz[i] = p[0];
  mesh->vertices.ambient_rgba[i] = c[0];
  i++;

  // right bottom
  mesh->vertices.xyz[i] = p[1];
  mesh->vertices.ambient_rgba[i] = c[1];
  i++;

  return 0;
}

int dx_asset_mesh_clear(dx_asset_mesh* mesh) {
  return resize_vertex_arrays(mesh, true, 0);
}

void dx_asset_mesh_set_mesh_ambient_rgba(dx_asset_mesh* mesh, DX_VEC4 const* value) {
  mesh->mesh.ambient_rgba = *value;
}
