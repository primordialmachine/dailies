#include "dx/asset/mesh/generators.h"

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
