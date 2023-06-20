#include "dx/core/math.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// sinf, cosf
#include <math.h>

// FLT_EPSILON
#include <float.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_RGB_U8 const dx_colors_capri = { 0, 191, 255 };

DX_RGB_U8 const dx_colors_malachite = { 0, 255, 64 };

DX_RGB_U8 const dx_colors_amber = { 255, 192, 0 };

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

float dx_vec3_sql(DX_VEC3 const* u) {
  return (u->e[0] * u->e[0])
    + (u->e[1] * u->e[1])
    + (u->e[2] * u->e[2]);
}

void dx_vec3_norm(DX_VEC3* u, DX_VEC3 const* v) {
  float sql = dx_vec3_sql(v);
  if (sql == 0.f) {
    u->e[0] = 0.f;
    u->e[1] = 0.f;
    u->e[2] = 0.f;
    return;
  }
  float l = sqrtf(sql);
  u->e[0] = v->e[0] / l;
  u->e[1] = v->e[1] / l;
  u->e[2] = v->e[2] / l;
}

void dx_vec3_add3(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v) {
  w->e[0] = u->e[0] + v->e[0];
  w->e[1] = u->e[1] + v->e[1];
  w->e[2] = u->e[2] + v->e[2];
}

void dx_vec3_sub3(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v) {
  w->e[0] = u->e[0] - v->e[0];
  w->e[1] = u->e[1] - v->e[1];
  w->e[2] = u->e[2] - v->e[2];
}

void dx_vec3_cross(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v) {
  float t[3];
  t[0] = u->e[1] * v->e[2] - u->e[2] * v->e[1];
  t[1] = u->e[2] * v->e[0] - u->e[0] * v->e[2];
  t[2] = u->e[0] * v->e[1] - u->e[1] * v->e[0];
  w->e[0] = t[0];
  w->e[1] = t[1];
  w->e[2] = t[2];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void dx_vec4_add4(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v) {
  w->e[0] = u->e[0] + v->e[0];
  w->e[1] = u->e[1] + v->e[1];
  w->e[2] = u->e[2] + v->e[2];
  w->e[3] = u->e[3] + v->e[3];
}

void dx_vec4_sub4(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v) {
  w->e[0] = u->e[0] - v->e[0];
  w->e[1] = u->e[1] - v->e[1];
  w->e[2] = u->e[2] - v->e[2];
  w->e[3] = u->e[3] - v->e[3];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void dx_mat4_set_identity(DX_MAT4* m) {
  // column #1
  m->e[0][0] = 1.f;
  m->e[1][0] = 0.f;
  m->e[2][0] = 0.f;
  m->e[3][0] = 0.f;
  
  // colum #2
  m->e[0][1] = 0.f;
  m->e[1][1] = 1.f;
  m->e[2][1] = 0.f;
  m->e[3][1] = 0.f;
  
  // column #3
  m->e[0][2] = 0.f;
  m->e[1][2] = 0.f;
  m->e[2][2] = 1.f;
  m->e[3][2] = 0.f;
  
  // column #4
  m->e[0][3] = 0.f;
  m->e[1][3] = 0.f;
  m->e[2][3] = 0.f;
  m->e[3][3] = 1.f;
}

void dx_mat4_set_translate(DX_MAT4* m, float x, float y, float z) {
  // column #1
  m->e[0][0] = 1.f;
  m->e[1][0] = 0.f;
  m->e[2][0] = 0.f;
  m->e[3][0] = 0.f;

  // colum #2
  m->e[0][1] = 0.f;
  m->e[1][1] = 1.f;
  m->e[2][1] = 0.f;
  m->e[3][1] = 0.f;

  // column #3
  m->e[0][2] = 0.f;
  m->e[1][2] = 0.f;
  m->e[2][2] = 1.f;
  m->e[3][2] = 0.f;

  // column #4
  m->e[0][3] = x;
  m->e[1][3] = y;
  m->e[2][3] = z;
  m->e[3][3] = 1.f;
}

void dx_mat4_set_rotate_x(DX_MAT4* a, float x) {
  x = dx_deg_to_rad(x);
  float c = cosf(x);
  float s = sinf(x);

  // column #1
  a->e[0][0] = 1.f;
  a->e[1][0] = 0.f;
  a->e[2][0] = 0.f;
  a->e[3][0] = 0.f;

  // column #2
  a->e[0][1] = 0.f;
  a->e[1][1] = c;
  a->e[2][1] = s;
  a->e[3][1] = 0.f;

  // column #2
  a->e[0][2] = 0.f;
  a->e[1][2] = -s;
  a->e[2][2] = c;
  a->e[3][2] = 0.f;

  // column #3
  a->e[0][3] = 0.f;
  a->e[1][3] = 0.f;
  a->e[2][3] = 0.f;
  a->e[3][3] = 1.f;
}

void dx_mat4_set_rotate_y(DX_MAT4* a, float x) {
  x = dx_deg_to_rad(x);
  float c = cosf(x);
  float s = sinf(x);

  // column #1
  a->e[0][0] = c;
  a->e[1][0] = 0.f;
  a->e[2][0] = -s;
  a->e[3][0] = 0.f;

  // column #2
  a->e[0][1] = 0.f;
  a->e[1][1] = 1.f;
  a->e[2][1] = 0.f;
  a->e[3][1] = 0.f;

  // column #2
  a->e[0][2] = s;
  a->e[1][2] = 0.f;
  a->e[2][2] = c;
  a->e[3][2] = 0.f;

  // column #3
  a->e[0][3] = 0.f;
  a->e[1][3] = 0.f;
  a->e[2][3] = 0.f;
  a->e[3][3] = 1.f;
}

void dx_mat4_set_ortho(DX_MAT4* m, float left, float right, float bottom, float top, float near, float far) {
  float a = right - left;
  float b = top  - bottom;
  float c = far - near;
  
  float u = -(right + left) / a;
  float v = -(top + bottom) / b;
  float w = -(far + near) / c;
  
  // column #1
  m->e[0][0] = 2.f / a;
  m->e[1][0] = 0.f;
  m->e[2][0] = 0.f;
  m->e[3][0] = 0.f;
  
  // column #2
  m->e[0][1] = 0.f;
  m->e[1][1] = 2.f / b;
  m->e[2][1] = 0.f;
  m->e[3][1] = 0.f;
  
  // column #3
  m->e[0][2] = 0.f;
  m->e[1][2] = 0.f;
  m->e[2][2] = -2.f / c;
  m->e[3][2] = 0.f;
  
  // column #4
  m->e[0][3] = u;
  m->e[1][3] = v;
  m->e[2][3] = w;
  m->e[3][3] = 1.f;
}

void dx_mat4_set_perspective(DX_MAT4* m, float field_of_vision, float aspect_ratio, float near, float far) {
  field_of_vision = field_of_vision * (DX_PI_F32 / 180.f); // rad(x) = x / 360 * 2 * PI = x * (PI * / 180)
  float f = 1.f / tanf(field_of_vision / 2.f); // cot(x) = 1 / tan(x)
  
  // column #1
  m->e[0][0] = f / aspect_ratio;
  m->e[1][0] = 0.f;
  m->e[2][0] = 0.f;
  m->e[3][0] = 0.f;

  // column #2
  m->e[0][1] = 0.f;
  m->e[1][1] = f;
  m->e[2][1] = 0.f;
  m->e[3][1] = 0.f;

  // column #3
  m->e[0][2] = 0.f;
  m->e[1][2] = 0.f;
  m->e[2][2] = (far + near) / (near - far);
  m->e[3][2] = -1.f;

  // column #4
  m->e[0][3] = 0.f;
  m->e[1][3] = 0.f;
  m->e[2][3] = (2.f * far * near) / (near - far);
  m->e[3][3] = 0.f;
}

void dx_mat4_mul3(DX_MAT4* c, DX_MAT4 const* a, DX_MAT4 const* b) {
  if (c!= a && c != b) {
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        c->e[i][j] = 0.f;
        for (size_t k = 0; k < 4; ++k) {
          c->e[i][j] += a->e[i][k] * b->e[k][j];
        }
      }
    }
  } else {
    float t[4][4];

    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        t[i][j] = 0.f;
        for (size_t k = 0; k < 4; ++k) {
          t[i][j] += a->e[i][k] * b->e[k][j];
        }
      }
    }
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        c->e[i][j] = t[i][j];
      }
    }
  }
}

void dx_mat4_set_look_at(DX_MAT4* a, DX_VEC3 const* source, DX_VEC3 const* target, DX_VEC3 const* up) {
  DX_VEC3 forward, right, up1, up2;
  DX_MAT4 r, t;

  // forward := norm(target - source)
  dx_vec3_sub3(&forward, target, source);
  dx_vec3_norm(&forward, &forward);
  // right := forward x norm(up)
  dx_vec3_norm(&up1, up);
  dx_vec3_cross(&right, &forward, &up1);
  // up' := right x forward
  dx_vec3_cross(&up2, &right, &forward);

  // column #1
  r.e[0][0] = right.e[0];
  r.e[1][0] = up2.e[0];
  r.e[2][0] = -forward.e[0];
  r.e[3][0] = 0.f;
  // column #2
  r.e[0][1] = right.e[1];
  r.e[1][1] = up2.e[1];
  r.e[2][1] = -forward.e[1];
  r.e[3][1] = 0.f;
  // column #3
  r.e[0][2] = right.e[2];
  r.e[1][2] = up2.e[2];
  r.e[2][2] = -forward.e[2];
  r.e[3][2] = 0.f;
  // column #4
  r.e[0][3] = 0.f;
  r.e[1][3] = 0.f;
  r.e[2][3] = 0.f;
  r.e[3][3] = 1.f;

  dx_mat4_set_translate(&t, -source->e[0], -source->e[1], -source->e[2]);

  dx_mat4_mul3(a, &r, &t);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void dx_transform_point(DX_VEC3* u, DX_VEC3 const* v, DX_MAT4 const* m) {
  float e[3];
  
  e[0] = m->e[0][0] * v->e[0]
       + m->e[0][1] * v->e[1]
       + m->e[0][2] * v->e[2]
       + m->e[0][3] * 1.f;
  
  e[1] = m->e[1][0] * v->e[0]
       + m->e[1][1] * v->e[1]
       + m->e[1][2] * v->e[2]
       + m->e[1][3] * 1.f;

  e[2] = m->e[2][0] * v->e[0]
       + m->e[2][1] * v->e[1]
       + m->e[2][2] * v->e[2]
       + m->e[2][3] * 1.f;

  u->e[0] = e[0];
  u->e[1] = e[1];
  u->e[2] = e[2];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
int dx_math_tests() {
  DX_VEC3 p = { 0.f, 0.f, -1.f };
  DX_VEC3 q;
  DX_MAT4 a;

#define EQ(x,y) dx_almost_equal_f32((x), (y), 2, FLT_EPSILON*2)
  
  // f is a counter-clockwise rotation around the y-axis by 0 degrees.
  // it follows that f((0,0,-1)) = (0,0,-1)
  dx_mat4_set_rotate_y(&a, 0.f);
  dx_transform_point(&q, &p, &a);
  if (!(EQ(q.e[0], 0.f) && EQ(q.e[1], 0.f) && EQ(q.e[2], -1.f))) {
    return 1;
  }

  // f is a counter-clockwise rotation around the y-axis by 90 degrees.
  // it follows that f((0,0,-1)) = (-1,0,0)
  dx_mat4_set_rotate_y(&a, 90.f);
  dx_transform_point(&q, &p, &a);
  if (!(EQ(q.e[0], -1.f) && EQ(q.e[1], 0.f) && EQ(q.e[2], 0.f))) {
    return 1;
  }
  
  // f is a counter-clockwise rotation around the y-xis by 180 degrees.
  // it follows that f((0,0,-1))= (0,0,1)
  dx_mat4_set_rotate_y(&a, 180.f);
  dx_transform_point(&q, &p, &a);
  if (!(EQ(q.e[0], 0.f) && EQ(q.e[1], 0.f) && EQ(q.e[2], 1.f))) {
    return 1;
  }
  
  // f is a counter-clockwise rotation around the y-xis by 270 degrees.
  // it follows that f((0,0,-1)) = (1,0,0)
  dx_mat4_set_rotate_y(&a, 270.f);
  dx_transform_point(&q, &p, &a);
  if (!(EQ(q.e[0], 1.f) && EQ(q.e[1], 0.f) && EQ(q.e[2], 0.f))) {
    return 1;
  }
#undef EQ
  return 0;
}
#endif
