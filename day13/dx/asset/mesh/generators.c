#include "dx/asset/mesh/generators.h"

int dx_asset_mesh_on_triangle(dx_asset_mesh* mesh) {
  static size_t const number_of_vertices = 3;
  static DX_VEC3 const xyz[] = {
    { -0.5f, -0.5f, 0.f, },
    { +0.5f, -0.5f, 0.f, },
    { +0.0f, +0.5f, 0.f, },
  };
  static DX_VEC4 const ambient_rgba[] = {
    { 1.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f },
  };
  for (size_t i = 0, n = number_of_vertices; i < n; ++i) {
    if (dx_asset_mesh_append_vertex(mesh, &xyz[i], &ambient_rgba[i])) {
      return 1;
    }
  }
  return 0;
}

int dx_asset_mesh_on_empty(dx_asset_mesh* mesh) {
  if (dx_asset_mesh_clear(mesh)) {
    return 1;
  }
  DX_VEC4 const COLOR = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  dx_asset_mesh_set_mesh_ambient_rgba(mesh, &COLOR);
  return 0;
}

int dx_asset_mesh_on_quadriliteral(dx_asset_mesh* mesh) {
  if (dx_asset_mesh_clear(mesh)) {
    return 1;
  }
  if (dx_asset_mesh_append_quadriliteral(mesh)) {
    return 1;
  }
  DX_VEC4 const COLOR = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  dx_asset_mesh_set_mesh_ambient_rgba(mesh, &COLOR);
  return 0;
}
