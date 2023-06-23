#if !defined(DX_COMMAND_H_INCLUDED)
#define DX_COMMAND_H_INCLUDED

#include "dx/val/vbinding.h"
#include "dx/val/cbinding.h"
#include "dx/val/material.h"
#include "dx/val/program.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// command that does nothing
#define DX_COMMAND_KIND_IDLE (0)

// command that clears the color buffer
#define DX_COMMAND_KIND_CLEAR_COLOR (1)

// command that clears the depth buffer
#define DX_COMMAND_KIND_CLEAR_DEPTH (2)

// command that draws triangles
#define DX_COMMAND_KIND_DRAW (3)

// command that sets the viewport
#define DX_COMMAND_KIND_VIEWPORT (4)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.command",
                       dx_command,
                       dx_object)

static inline dx_command* DX_COMMAND(void* p) {
  return (dx_command*)p;
}

int dx_command_construct_clear_color(dx_command* render_command, float l, float b, float w, float h, DX_VEC4 const* color);

/// Create a clear color (buffer) command.
/// @param color The value to clear the color buffer with.
/// @return A pointer to the command.
dx_command* dx_command_create_clear_color(float l, float b, float w, float h, DX_VEC4 const* color);

int dx_command_construct_clear_depth(dx_command* render_command, float l, float b, float w, float h, float depth);

/// Create a clear color (buffer) command.
/// @param depth The value to clear the color buffer with.
/// @return A pointer to the command.
dx_command* dx_command_create_clear_depth(float l, float b, float w, float h, float depth);

int dx_command_construct_draw(dx_command* render_command, dx_vbinding* vbinding, dx_material* material, dx_cbinding* cbinding, dx_program* program, int start, int length);

/// Create a draw command.
/// @param vbinding The variable binding.
/// @param material The material.
/// @param cbinding The constant binding.
/// @param program The program.
/// @param start The index at which the vertex sequence starts at.
/// @param length The length of the vertex sequence.
/// @return A pointer to the command.
dx_command* dx_command_create_draw(dx_vbinding* vbinding, dx_material* material, dx_cbinding* cbinding, dx_program* program, int start, int length);

int dx_command_construct_viewport(dx_command* render_command, float l, float b, float w, float h);

/// Create a viewport command.
/// @param binding The binding.
/// @param program The program.
/// @param l, b position along the x- (left) and y-axis (bottom) of the viewport.
/// @param w, h extends along the x- (width) and y-axis (height) of the viewport.
/// @return A pointer to the command.
/// @remarks
/// This assumes
/// - the origin (0,0) maps to the left bottom of the canvas.
/// - the positive x-axis goes from the left side of the canvas to the right side.
/// - the positive y-axis goes from the bottom side of the canvas to the top side.
dx_command* dx_command_create_viewport(float l, float b, float w, float h);

void dx_command_destruct(dx_command* render_command);

struct dx_command {
  dx_object _parent;
  uint8_t kind;
  union {
    struct {
      struct {
        float l, b, w, h;
      } rectangle;
      DX_VEC4 color;
    } clear_color_command;
    struct {
      struct {
        float l, b, w, h;
      } rectangle;
      float depth;
    } clear_depth_command;
    struct {
      dx_vbinding* vbinding;
      dx_material* material;
      dx_cbinding* cbinding;
      dx_program* program;
      int start;
      int length;
    } draw_command;
    struct {
      float l;
      float b;
      float w;
      float h;
    } viewport_command;
  };
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.command_list",
                       dx_command_list,
                       dx_object)

static inline dx_command_list* DV_COMMAND_LIST(void* p) {
  return (dx_command_list*)p;
}

int dx_command_list_construct(dx_command_list* command_list);

void dx_command_list_destruct(dx_command_list* command_list);

dx_command_list* dx_command_list_create();

int dx_command_list_append(dx_command_list* command_list, dx_command* command);

size_t dx_command_list_get_size(dx_command_list const* command_list);

dx_command* dx_command_list_get_at(dx_command_list const* command_list, size_t index);

struct dx_command_list {
  dx_object _parent;
  size_t size, capacity;
  dx_command** elements;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_COMMAND_H_INCLUDED
