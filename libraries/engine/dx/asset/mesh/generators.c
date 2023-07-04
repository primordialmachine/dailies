#include "dx/asset/mesh/generators.h"

int dx_asset_mesh_on_triangle(dx_asset_mesh* mesh) {
  static dx_size const number_of_vertices = 3;
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
  static DX_VEC2 const ambient_uv[] = {
    { 0.f, 0.f, },
    { 1.f, 0.f, },
    { 0.f, 1.f, },
  };
  for (dx_size i = 0, n = number_of_vertices; i < n; ++i) {
    if (dx_asset_mesh_append_vertex(mesh, &xyz[i], &ambient_rgba[i], &ambient_uv[i])) {
      return 1;
    }
  }
  return 0;
}

int dx_asset_mesh_on_cube(dx_asset_mesh* mesh) {
  if (dx_asset_mesh_clear(mesh)) {
    return 1;
  }
  DX_MAT4 t;
  dx_mat4_set_translate(&t, 0, 0, +0.5f);

  DX_MAT4 r;
  dx_size i = 0;
  // front face
  dx_asset_mesh_append_quadriliteral(mesh);
  dx_asset_mesh_transform_range(mesh, &t, i, 6);
  i += 6;
  // back face (rotate front face counter-clockwise around the y-axis by -180 (or +180) degrees)
  dx_mat4_set_rotate_y(&r, 180);
  dx_asset_mesh_append_quadriliteral(mesh);
  dx_asset_mesh_transform_range(mesh, &t, i, 6);
  dx_asset_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  // left face (rotate front face counter-clockwise around the y-axis by -90 degrees)
  dx_mat4_set_rotate_y(&r, -90);
  dx_asset_mesh_append_quadriliteral(mesh);
  dx_asset_mesh_transform_range(mesh, &t, i, 6);
  dx_asset_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  // right face (rotate front face counter-clockwise around the y-axis by +90 degrees)
  dx_mat4_set_rotate_y(&r, +90);
  dx_asset_mesh_append_quadriliteral(mesh);
  dx_asset_mesh_transform_range(mesh, &t, i, 6);
  dx_asset_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  // bottom face (rotate front face counter-clockwise around the x-axis by -90 degrees)
  dx_mat4_set_rotate_x(&r, -90);
  dx_asset_mesh_append_quadriliteral(mesh);
  dx_asset_mesh_transform_range(mesh, &t, i, 6);
  dx_asset_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  // top face (rotate front face counter-clockwise around the x-axis by 90 degrees)
  dx_mat4_set_rotate_x(&r, +90);
  dx_asset_mesh_append_quadriliteral(mesh);
  dx_asset_mesh_transform_range(mesh, &t, i, 6);
  dx_asset_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  //
  DX_VEC4 const COLOR = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  dx_asset_mesh_set_mesh_ambient_rgba(mesh, &COLOR);
  //
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

int dx_asset_mesh_on_octahedron(dx_asset_mesh* mesh) {
  if (dx_asset_mesh_clear(mesh)) {
    return 1;
  }
  DX_MAT4 t;

  DX_MAT4 r;
  dx_size i = 0;

  static DX_VEC4 const WHITE = { 1.f, 1.f, 1.f, 1.f };

  static DX_VEC3 const top = { 0.f, +0.5f, 0.f };
  static DX_VEC2 const top_uv = { 0.f, 1.f };

  static DX_VEC3 const bottom = { 0.f, -0.5f, 0.f };
  static DX_VEC2 const bottom_uv = { 0.f, 0.f };

  static DX_VEC3 const left = { -0.5f, 0.f, 0.f };
  static DX_VEC2 const left_uv = { 0.f, 1.f };

  static DX_VEC3 const right = { +0.5f, 0.f, 0.f };

  static DX_VEC3 const front = { 0.f, 0.f, +0.5f };
  static DX_VEC3 const back = { 0.f, 0.f, -0.5f };

  DX_VEC4 rgba = { 1, 1, 1, 1 };
  // left front
  {
    DX_VEC3 xyz = { -1.f, 0, 0 };
    DX_VEC2 uv = { 0, 0 };
    dx_asset_mesh_append_vertex(mesh, &xyz, &rgba, &uv);
  }
  {
    DX_VEC3 xyz = { 0, 0, 1.f };
    DX_VEC2 uv = { 1, 0 };
    dx_asset_mesh_append_vertex(mesh, &xyz, &rgba, &uv);
  }
  {
    DX_VEC3 xyz = { 0, 1.f, 0 };
    DX_VEC2 uv = { 0, 1 };
    dx_asset_mesh_append_vertex(mesh, &xyz, &rgba, &uv);
  }
  // right front
  {
    dx_asset_mesh_append_range(mesh, 0, 3);
    DX_MAT4 r;
    dx_mat4_set_rotate_y(&r, 90);
    dx_asset_mesh_transform_range(mesh, &r, 3, 3);
  }
  // right back
  {
    dx_asset_mesh_append_range(mesh, 3, 3);
    DX_MAT4 r;
    dx_mat4_set_rotate_y(&r, 90);
    dx_asset_mesh_transform_range(mesh, &r, 6, 3);
  }
  // left back
  {
    dx_asset_mesh_append_range(mesh, 6, 3);
    DX_MAT4 r;
    dx_mat4_set_rotate_y(&r, 90);
    dx_asset_mesh_transform_range(mesh, &r, 9, 3);
  }
  // bottom
  {
    dx_asset_mesh_append_range(mesh, 0, 12);
    DX_MAT4 r;
    dx_mat4_set_rotate_x(&r, -180);
    dx_asset_mesh_transform_range(mesh, &r, 12, 12);
  }

  //
  DX_VEC4 const COLOR = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  dx_asset_mesh_set_mesh_ambient_rgba(mesh, &COLOR);
  //
  return 0;
}
