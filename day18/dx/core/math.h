#if !defined(DX_MATH_H_INCLUDED)
#define DX_MATH_H_INCLUDED

#include "dx/core/core.h"

typedef struct DX_VEC3 DX_VEC3;
typedef struct DX_VEC4 DX_VEC4;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_RGB_U8 {
  uint8_t r, g, b;
} DX_RGB_U8;

// The color "Capri"  (0, 191, 255).
extern DX_RGB_U8 const dx_colors_capri;

// The color "Malachite" (0, 255, 64) from "Capri"'s tetradic palette.
extern DX_RGB_U8 const dx_colors_malachite;

// The color "Amber" (255, 192, 0) from "Capri"'s Split Complementary palette.
extern DX_RGB_U8 const dx_colors_amber;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Symbolic constant for float representation of PI.
#define DX_PI_F32 3.1415926f

// @brief Convert an angle in degrees into the corresponding angle in radians.
// @param a The angle in degrees.
// @return The corresponding angle in radians.
static inline float dx_deg_to_rad(float x) {
  return (x / 180.f) * DX_PI_F32;
}

// @brief Clamp a value to the range [0,1].
// @param x The value to be clamped.
// @return x clamped.
static inline float dx_clamp(float x) {
  if (x < 0.f) return 0.f;
  else if (x > 1.f) return 1.f;
  else return x;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_VEC2 {
  float e[2];
} DX_VEC2;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_VEC3 {
  float e[3];
} DX_VEC3;

static inline void dx_vec3_set(DX_VEC3* v, float x, float y, float z) {
  v->e[0] = x;
  v->e[1] = y;
  v->e[2] = z;
}

// get the squared length of a vector
// @param v the vector
// @return the squared length of the vector
float dx_vec3_sql(DX_VEC3 const* v);

// Compute a normalized vector.
// @param v the vector receiving the normalized vector
// @param u the vector to normalize 
// @remarks The result is the normalized vector of the vector to be normalized if the vector to be normalized is a non-zero vector.
// Otherwise the result is the zero vector.
// @a v and @a u may refer to the same object.
void dx_vec3_norm(DX_VEC3* v, DX_VEC3 const* u);

/// @ingroup math
/// @brief Compute the sum of two vectors.
/// @param w Pointer too a DX_VEC3 object.
/// @param u Pointer to a DX_VEC3 object.
/// The object's values represent the augend (aka the 1st operand).
/// @param v Pointer toa DX_VEC3 object.
/// The object's values represent the addend (aka the 2nd operand).
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the sum vector.
void dx_vec3_add3(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v);

/// @ingroup math
/// @brief Compute the difference of two vectors.
/// @param w Pointer to a DX_VEC3 object.
/// @param u Pointer to a DX_VEC3 object.
/// The object's values represent the vector that is the minuend (aka the 1st operand).
/// @param v Pointer to a DX_VEC3 object.
/// The object's values represent hte vector that is the the subtrahend (aka the 2nd operand).
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the difference vector.
void dx_vec3_sub3(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v);

/// @ingroup math
/// @brief Compute the cross product of two vectors.
/// @param w Pointer to a DX_VEC3 object.
/// @param u Pointer to a DX_VEC3 object.
/// The object's values represent the vector, that is the 1st operand.
/// @param v Pointer to a DX_VEC3 object.
/// The object's values represent the vector, that is the 2nd operand.
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the cross product vector.
void dx_vec3_cross(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_VEC4 {
  float e[4];
} DX_VEC4;

static inline void dx_vec4_lerp(DX_VEC4 const* u, DX_VEC4 const* v, float t, DX_VEC4* w) {
  t = dx_clamp(t);
  if (t == 0.f) {
    *w = *u;
  } else if (t == 1.f) {
    *w = *v;
  } else {
    w->e[0] = (1.f - t) * u->e[0] + t * v->e[0];
    w->e[1] = (1.f - t) * u->e[1] + t * v->e[1];
    w->e[2] = (1.f - t) * u->e[2] + t * v->e[2];
    w->e[3] = (1.f - t) * u->e[3] + t * v->e[3];
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// A 4x4 single precision floating point matrix POD.
typedef struct DX_MAT4 {
  // index i,j denotes i-th row and j-th colmn
  float e[4][4];
} DX_MAT4;

// assign this matrix the values of an identity matrix
void dx_mat4_set_identity(DX_MAT4* m);

// assign this matrix the values of a translation matrix
void dx_mat4_set_translate(DX_MAT4* m, float x, float y, float z);

// assign this matrix the values of transformation matrix representing
// a counter-clockwise rotation around the x-axis by the specified number of degrees.
// @param m A pointer to this matrix.
// @param a The angle of rotation, in degrees.
void dx_mat4_set_rotate_x(DX_MAT4* a, float x);

// assign this matrix the values of transformation matrix representing
// a counter-clockwise rotation around the y-axis by the specified number of degrees.
// @param m A pointer to this matrix.
// @param a The angle of rotation, in degrees.
void dx_mat4_set_rotate_y(DX_MAT4* a, float x);

// assign this matrix the values of an orthographic projection matrix such that
// - the positive z-axis points out of the screen (negative z-axis points into the screen)
// - the positive x-axis points to the right
// - the positive y-axis points to the top
void dx_mat4_set_ortho(DX_MAT4* m, float left, float right, float bottom, float top, float near, float far);

// assign this matrix the value of perspective projection matrix such that
// - the positve
void dx_mat4_set_perspective(DX_MAT4* m, float field_of_vision, float aspect_ratio, float near, float far);

/// Compute the product of two matrices.
/// @param c Pointer to a DX_MAT4 object.
/// @param a Pointer to a DX_MAT4 object.
/// The object's values represent the matrix that is the the multiplier (aka the 1st factor aka the 1st operand)
/// @param b Pointer to a DX_MAT4 object.
/// The object's values represent the matrix that is the multiplicand (aka the 2nd factor aka the 2nd operand).
/// @remarks @a c, @a b, and @a b all may refer to the same object.
/// @post <code>*c/<code> was assigned the values of the product matrix.
void dx_mat4_mul3(DX_MAT4* c, DX_MAT4 const* a, DX_MAT4 const* b);

/// Assign this matrix the value a of a view matrix.
/// @param source the position vector of the point at which the viewer is located
/// @param target the position vector of the point at which the viewer is looking at
/// @param uo the direction vector indicating the upward direction of the viewer
/// @remarks
/// This function constructs a view matrix <code>V</code>given
/// - the position the viewer is located at <code>source</code>,
/// - the position the viewer is looking at <code>target</code>, and
/// - the vector indicating the up direction of the viewer <code>up</code>.
/// The view matrix <code>V</code> is constructed as follows
/// Let
/// @code
/// forward := norm(target - source)
/// right := forward x norm(up)
/// up' := right x forward
/// @endcode
/// Then the view matrix <code>V</code> is given by
/// @code
/// V :=
/// | right.x    | right.y    | right.z    | 0
/// | up'.x      | up'.y      | u'.z       | 0
/// | -forward.x | -forward.y | -forward.z | 0
/// | 0          | 0          | 0          | 1
/// @endcode
void dx_mat4_set_look_at(DX_MAT4* a, DX_VEC3 const* source, DX_VEC3 const* target, DX_VEC3 const* up);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup math
/// Transform a position vector.
/// @param u Pointer to a DX_VEC3 object. The object receives the transformed position vector.
/// @param v Pointer to a DX_VEC3 object. The object represents the position to transform.
/// @param m Pointer to a DX_MAT4 object. The object represents the transformation matrix.
void dx_transform_point(DX_VEC3* u, DX_VEC3 const* v, DX_MAT4 const* m);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
int dx_math_tests();
#endif

#endif // DX_MATH_H_INCLUDED
