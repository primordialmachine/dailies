#include "dx/scenes/mesh_viewer_scene.h"

// malloc, free
#include <malloc.h>
// _strdup
#include <string.h>
// sinf
#include <math.h>
// FLT_MAX
#include <float.h>
// snprintf
#include <stdio.h>
#include "dx/program_text.h"
#include "dx/texture.h"
#include "dx/program.h"
#include "dx/cbinding.h"

static dx_asset_texture* create_checkerbord_texture() {
  // Create a checkerboard texture.
  DX_ASSET_CHECKERBOARD_BRUSH brush;
  brush._parent.flags = DX_ASSET_BRUSH_FLAGS_CHECKBERBOARD;
  brush.number_of_checkers.horizontal = 16;
  brush.number_of_checkers.vertical = 16;
  brush.checker_size.horizontal = 128;
  brush.checker_size.vertical = 128;
  brush.checker_colors.first = dx_colors_malachite;
  brush.checker_colors.second = dx_colors_amber;
  struct {
    size_t horizontal;
    size_t vertical;
  } number_of_pixels = {
    .horizontal = brush.number_of_checkers.horizontal * brush.checker_size.horizontal,
    .vertical = brush.number_of_checkers.vertical * brush.checker_size.vertical,
  };
  dx_asset_image* asset_image = dx_asset_image_create_rgb_u8(DX_PIXEL_FORMAT_RGB_U8, number_of_pixels.horizontal, number_of_pixels.vertical, &dx_colors_malachite);
  if (!asset_image) {
    return NULL;
  }
  if (dx_asset_image_fill(asset_image, 0, 0, asset_image->width, asset_image->height, (DX_ASSET_BRUSH const*)&brush)) {
    DX_UNREFERENCE(asset_image);
    asset_image = NULL;
    return NULL;
  }
  dx_asset_texture* asset_texture = dx_asset_texture_create(asset_image);
  DX_UNREFERENCE(asset_image);
  asset_image = NULL;
  return asset_texture;
}

static dx_cbinding* create_cbinding() {
  DX_MAT4 identity;
  dx_mat4_set_identity(&identity);
  dx_cbinding* cbinding = dx_cbinding_create();
  if (!cbinding) {
    return NULL;
  }
  static const DX_VEC4 malachite = { 0.f, 255.f / 255.f, 64.f / 255.f, 1.0f }; // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
  dx_cbinding_set_vec4(cbinding, "vs_mesh_ambient_rgba", &malachite);
  dx_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &identity);
  dx_cbinding_set_mat4(cbinding, "matrices.view_matrix", &identity);
  dx_cbinding_set_mat4(cbinding, "vs_mesh_world_matrix", &identity);
  dx_cbinding_set_texture_index(cbinding, "ambient_texture_sampler", 0);
  return cbinding;
}

// @brief Load the fragment shader <version>.fs and the vertex shader <version>.vs as a program text object.
// @remarks
// <version>.fs is a fragment shader and <version>.fs is a vertex shader
// where <version> can assume the values 1 and 2. The shaders differ in
// what color information they use.
// - 1 the shader requires per-mesh color information.
// - 2 the shader requires per-vertex color information.
static dx_program_text* load_program(dx_string *path, dx_string* filename) {
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

// the initial position of the viewer.
// must be different from TARGET.
static DX_VEC3 const SOURCE = { 0.f, 0.f, 1.f };

// Rotate viewer with 25 degrees per second counter-clockwise around the y-axis.
static float const degrees_per_second = 25.f;

// Let viewer look at this point.
static DX_VEC3 const TARGET = { 0.f, 0.f, 0.f };

// The viewer's up vector.
static DX_VEC3 const UP = { 0.f, 1.f, 0.f };

static int update_viewer(dx_mesh_viewer_scene* scene) {
  DX_MAT4 a;
  dx_mat4_set_rotate_y(&a, scene->angle);
  dx_transform_point(&scene->viewer_position, &SOURCE, &a);
  dx_mat4_set_look_at(&scene->view_matrix, &scene->viewer_position, &TARGET, &UP);
  return 0;
}

// add commands "clear color", "clear depth", and "set viewport".
static int add_enter_frame_commands(dx_command_list* commands) {
  dx_command* command;
  // clear color command
  static const DX_VEC4 clear_color = { 0.f / 255.f, 191.f / 255.f, 255.f / 255.f, 0.f }; // color called "Capri" (0, 191, 255)
  command = dx_command_create_clear_color(0, 0, 640, 480, &clear_color);
  if (!command) {
    return 1;
  }
  if (dx_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return 1;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  // clear depth command
  static const float clear_depth = 1.f;
  command = dx_command_create_clear_depth(0, 0, 640, 480, 1.f);
  if (!command) {
    return 1;
  }
  if (dx_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return 1;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  // set viewport command
  command = dx_command_create_viewport(0, 0, 640, 480);
  if (!command) {
    return 1;
  }
  if (dx_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return 1;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  //
  return 0;
}

static int dx_mesh_viewer_scene_startup(dx_mesh_viewer_scene* scene, dx_context* context) {
#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
  if (dx_math_tests()) {
    return 1;
  }
#endif
#if defined(DX_SAFE_ADD_NX_WITH_TESTS) && 1 == DX_SAFE_ADD_NX_WITH_TESTS
  if (dx_safe_add_nx_tests()) {
    return 1;
  }
#endif
#if defined(DX_SAFE_MUL_NX_WITH_TESTS) && 1 == DX_SAFE_MUL_NX_WITH_TESTS
  if (dx_safe_mul_nx_tests()) {
    return 1;
  }
#endif

  scene->angle = 0.f;

  dx_vbinding* vbinding = NULL;
  dx_program* program = NULL;
  dx_buffer* buffer = NULL;
  dx_asset_mesh* mesh = NULL;
  //
  dx_mat4_set_identity(&scene->projection_matrix);
  update_viewer(scene);
  dx_mat4_set_identity(&scene->world_matrix);
  //
  {
    dx_string* name1 = dx_string_create(scene->name, strlen(scene->name));
    if (!name1) {
      return 1;
    }
    mesh = dx_asset_mesh_create(name1);
    DX_UNREFERENCE(name1);
    name1 = NULL;
    if (!mesh) {
      return 1;
    }
    if (scene->on_mesh_loaded) {
      if (scene->on_mesh_loaded(mesh)) {
        DX_UNREFERENCE(mesh);
        return 1;
      }
    }
  }
  //
  static const DX_VERTEX_FORMAT vertex_format = DX_VERTEX_FORMAT_POSITION_TEXTURE;
  {
    buffer = dx_context_create_buffer(context);
    if (!buffer) {
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
    void* bytes; size_t number_of_bytes;
    if (dx_asset_mesh_format(mesh, vertex_format, &bytes, &number_of_bytes)) {
      DX_UNREFERENCE(buffer);
      buffer = NULL;
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
    if (dx_buffer_set_data(buffer, bytes, number_of_bytes)) {
      free(bytes);
      bytes = NULL;
      DX_UNREFERENCE(buffer);
      buffer = NULL;
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
    free(bytes);
    bytes = NULL;
  }
  //
  {
    vbinding = dx_context_create_vbinding(context, vertex_format, buffer);
    if (!vbinding) {
      DX_UNREFERENCE(buffer);
      buffer = NULL;
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
    DX_UNREFERENCE(buffer);
    buffer = NULL;
  }
  //
  {
    dx_string* path = dx_string_create("assets/gl", sizeof("assets/gl")-1);
    if (!path) {
      DX_UNREFERENCE(vbinding);
      vbinding = NULL;
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
    dx_string* filename = dx_string_create("3", sizeof("3") - 1);
    if (!filename) {
      DX_UNREFERENCE(path);
      path = NULL;
      DX_UNREFERENCE(vbinding);
      vbinding = NULL;
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
    dx_program_text* program_text = load_program(path, filename);
    DX_UNREFERENCE(filename);
    filename = NULL;
    DX_UNREFERENCE(path);
    path = NULL;
    if (!program_text) {
      DX_UNREFERENCE(vbinding);
      vbinding = NULL;
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
    program = dx_context_create_program(context, program_text);
    DX_UNREFERENCE(program_text);
    program_text = NULL;
    if (!program) {
      DX_UNREFERENCE(vbinding);
      vbinding = NULL;
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
  }
  //
  {
    dx_command* command;
    dx_command_list* commands = dx_command_list_create();
    if (!commands) {
      return 1;
    }
    if (add_enter_frame_commands(commands)) {
      DX_UNREFERENCE(commands);
      commands = NULL;
      DX_UNREFERENCE(program);
      program = NULL;
      DX_UNREFERENCE(vbinding);
      vbinding = NULL;
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
    dx_cbinding* cbinding = create_cbinding();
    if (!cbinding) {
      DX_UNREFERENCE(commands);
      commands = NULL;
      DX_UNREFERENCE(program);
      program = NULL;
      DX_UNREFERENCE(vbinding);
      vbinding = NULL;
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return 1;
    }
    // draw command
    command = dx_command_create_draw(vbinding, cbinding, program, 0, mesh->number_of_vertices);
    DX_UNREFERENCE(cbinding);
    cbinding = NULL;
    DX_UNREFERENCE(program);
    program = NULL;
    DX_UNREFERENCE(vbinding);
    vbinding = NULL;
    DX_UNREFERENCE(mesh);
    mesh = NULL;
    if (!command) {
      DX_UNREFERENCE(commands);
      commands = NULL;
      return 1;
    }
    if (dx_command_list_append(commands, command)) {
      DX_UNREFERENCE(command);
      command = NULL;
      DX_UNREFERENCE(commands);
      commands = NULL;
      return 1;
    }
    DX_UNREFERENCE(command);
    command = NULL;
    scene->commands = commands;
  }
  //
  {
    // Create a checkerboard texture.
    dx_asset_texture* asset_texture = create_checkerbord_texture();
    if (!asset_texture) {
      DX_UNREFERENCE(scene->commands);
      scene->commands = NULL;
      return 1;
    }
    dx_texture* texture = dx_context_create_texture(context);
    if (!texture) {
      DX_UNREFERENCE(asset_texture);
      asset_texture = NULL;
      DX_UNREFERENCE(scene->commands);
      scene->commands = NULL;
      return 1;
    }
    if (dx_texture_set_data(texture, asset_texture)) {
      DX_UNREFERENCE(texture);
      texture = NULL;
      DX_UNREFERENCE(asset_texture);
      asset_texture = NULL;
      DX_UNREFERENCE(scene->commands);
      scene->commands = NULL;
      return 1;
    }
    DX_UNREFERENCE(asset_texture);
    asset_texture = NULL;
    scene->texture = texture;
  }
  //
  return 0;
}

static int dx_mesh_viewer_scene_render(dx_mesh_viewer_scene* scene, dx_context* context, float delta_seconds, int canvas_width, int canvas_height) {
  dx_mat4_set_perspective(&scene->projection_matrix, 60.f, (float)canvas_width / (float)canvas_height, +0.1f, 100.f);
  update_viewer(scene);

  float degrees = degrees_per_second * delta_seconds;
  scene->angle = fmodf(scene->angle + degrees, 360.f);
  {
    dx_command* command;

    // Update the rectangle of the "clear color" command.
    command = dx_command_list_get_at(scene->commands, 0);
    if (DX_COMMAND_KIND_CLEAR_COLOR != command->kind) {
      return 1;
    }
    command->clear_color_command.rectangle.w = (float)canvas_width;
    command->clear_color_command.rectangle.h = (float)canvas_height;

    // Update the rectangle of the "clear depth" command.
    command = dx_command_list_get_at(scene->commands, 1);
    if (DX_COMMAND_KIND_CLEAR_DEPTH != command->kind) {
      return 1;
    }
    command->clear_depth_command.rectangle.w = (float)canvas_width;
    command->clear_depth_command.rectangle.h = (float)canvas_height;

    // Update the rectangle of the "set viewport" command.
    command = dx_command_list_get_at(scene->commands, 2);
    if (DX_COMMAND_KIND_VIEWPORT != command->kind) {
      return 1;
    }
    command->viewport_command.w = (float)canvas_width;
    command->viewport_command.h = (float)canvas_height;
  }
  {
    // bind the constants
    dx_command* command = dx_command_list_get_at(scene->commands, 3);
    if (DX_COMMAND_KIND_DRAW != command->kind) {
      return 1;
    }
    dx_cbinding* cbinding = command->draw_command.cbinding;
    dx_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &scene->projection_matrix);
    dx_cbinding_set_mat4(cbinding, "matrices.view_matrix", &scene->view_matrix);
    dx_cbinding_set_mat4(cbinding, "vs_mesh_world_matrix", &scene->world_matrix);
  }
  {
    dx_context_bind_texture(context, 0, scene->texture);
  }

  return dx_context_execute_commands(context, scene->commands);
}

static int dx_mesh_viewer_scene_shutdown(dx_mesh_viewer_scene* scene, dx_context* context) {
  if (scene->texture) {
    DX_UNREFERENCE(scene->texture);
    scene->texture = NULL;
  }
  if (scene->commands) {
    DX_UNREFERENCE(scene->commands);
    scene->commands = NULL;
  }
  return 0;
}

int dx_mesh_viewer_scene_construct(dx_mesh_viewer_scene* scene, char const *name, int (*on_mesh_loaded)(dx_asset_mesh*)) {
  if (dx_scene_construct(DX_SCENE(scene))) {
    return 1;
  }
  scene->name = _strdup(name);
  if (!scene->name) {
    dx_scene_destruct(DX_SCENE(scene));
    return 1;
  }
  scene->on_mesh_loaded = on_mesh_loaded;
  dx_mat4_set_identity(&scene->projection_matrix);
  dx_mat4_set_identity(&scene->view_matrix);
  dx_mat4_set_identity(&scene->projection_matrix);
  scene->commands = NULL;
  scene->texture = NULL;
  DX_SCENE(scene)->startup = (int (*)(dx_scene*, dx_context*)) & dx_mesh_viewer_scene_startup;
  DX_SCENE(scene)->render = (int (*)(dx_scene*, dx_context*, float, int, int)) &dx_mesh_viewer_scene_render;
  DX_SCENE(scene)->shutdown = (int (*)(dx_scene*, dx_context*)) dx_mesh_viewer_scene_shutdown;
  DX_OBJECT(scene)->destruct = (void (*)(dx_object*)) & dx_mesh_viewer_scene_destruct;
  return 0;
}

void dx_mesh_viewer_scene_destruct(dx_mesh_viewer_scene* scene) {
  if (scene->name) {
    free(scene->name);
    scene->name = NULL;
  }
  dx_scene_destruct(DX_SCENE(scene));
}

dx_mesh_viewer_scene* dx_mesh_viewer_scene_create(char const* name, int (*on_mesh_loaded)(dx_asset_mesh*)) {
  dx_mesh_viewer_scene* scene = DX_MESH_VIEWER_SCENE(dx_object_alloc(sizeof(dx_mesh_viewer_scene)));
  if (!scene) {
    return NULL;
  }
  if (dx_mesh_viewer_scene_construct(scene, name, on_mesh_loaded)) {
    DX_UNREFERENCE(scene);
    scene = NULL;
    return NULL;
  }
  return scene;
}
