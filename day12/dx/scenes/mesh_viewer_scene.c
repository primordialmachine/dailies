#include "dx/scenes/mesh_viewer_scene.h"

// _strdup
#include <string.h>
// sinf
#include <math.h>
// snprintf
#include <stdio.h>
#include "dx/val/cbinding.h"
#include "dx/scenes/create_assets.h"
#include "dx/val/viewer.h"
#include "dx/ddl.h"
#include "dx/asset/optics.h"

DX_DEFINE_OBJECT_TYPE("dx.mesh_viewer_scene",
                      dx_mesh_viewer_scene,
                      dx_scene)

static int on_scene_asset_object(dx_mesh_viewer_scene* self, dx_context* context, dx_object* asset_object) {
  // mesh instance
  if (dx_rti_type_is_leq(asset_object->type, dx_asset_mesh_instance_get_type())) {
    dx_asset_mesh_instance* asset_mesh_instance = DX_ASSET_MESH_INSTANCE(asset_object);
    dx_mesh* mesh = dx_mesh_create(context, DX_ASSET_MESH(asset_mesh_instance->mesh_reference->object));
    dx_val_mesh_instance* mesh_instance = dx_val_mesh_instance_create(asset_mesh_instance->world_matrix, mesh);
    if (dx_object_array_append(&self->mesh_instances, DX_OBJECT(mesh_instance))) {
      DX_UNREFERENCE(mesh_instance);
      mesh_instance = NULL;
      return 1;
    }
    DX_UNREFERENCE(mesh_instance);
    mesh_instance = NULL;
    return DX_NO_ERROR != dx_get_error();
  }
  if (dx_get_error()) {
    return 1;
  }
  // viewer instance
  if (dx_rti_type_is_leq(asset_object->type, dx_asset_viewer_instance_get_type())) {
    dx_asset_viewer_instance* asset_viewer_instance = DX_ASSET_VIEWER_INSTANCE(asset_object);
    dx_val_viewer* viewer = dx_val_viewer_create(asset_viewer_instance);
    if (dx_object_array_append(&self->viewers, DX_OBJECT(viewer))) {
      DX_UNREFERENCE(viewer);
      viewer = NULL;
      return 1;
    }
    DX_UNREFERENCE(viewer);
    viewer = NULL;
    return DX_NO_ERROR != dx_get_error();
  }
  if (dx_get_error()) {
    return 1;
  }
  dx_set_error(DX_SEMANTICAL_ERROR);
  dx_log("unknown/unsupported scene object", sizeof("unknown/unsupported scene object") - 1);
  return 1;
}

static int mesh_instance_on_startup(dx_mesh_viewer_scene* self, dx_context* context, dx_asset_scene* asset_scene) {
  if (dx_object_array_initialize(&self->mesh_instances, 2)) {
    return 1;
  }
  if (dx_object_array_initialize(&self->viewers, 1)) {
    dx_object_array_uninitialize(&self->mesh_instances);
    return 1;
  }
  for (dx_size i = 0, n = dx_object_array_get_size(&asset_scene->mesh_instances); i < n; ++i) {
    dx_object* asset_object = dx_object_array_get_at(&asset_scene->mesh_instances, i);
    if (!asset_object) {
      dx_object_array_uninitialize(&self->viewers);
      dx_object_array_uninitialize(&self->mesh_instances);
      return 1;
    }
    if (on_scene_asset_object(self, context, asset_object)) {
      dx_object_array_uninitialize(&self->viewers);
      dx_object_array_uninitialize(&self->mesh_instances);
      return 1;
    }
  }
  return 0;
}

static void mesh_instance_on_shutdown(dx_mesh_viewer_scene* self) {
  dx_object_array_uninitialize(&self->viewers);
  dx_object_array_uninitialize(&self->mesh_instances);
}

static int viewer_push_constants(dx_mesh_viewer_scene* self, dx_cbinding* cbinding, dx_i32 canvas_width, dx_i32 canvas_height) {
  dx_val_viewer* viewer = DX_VAL_VIEWER(dx_object_array_get_at(&self->viewers, dx_object_array_get_size(&self->viewers) - 1));
  if (!viewer) {
    return 1;
  }
  DX_MAT4 a;
  if (dx_val_viewer_get_projection_matrix(&a, viewer, canvas_width, canvas_height)) {
    return 1;
  }
  dx_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &a);
  if (dx_val_viewer_get_view_matrix(&a, viewer, canvas_width, canvas_height)) {
    return 1;
  }
  dx_cbinding_set_mat4(cbinding, "matrices.view_matrix", &a);
  return 0;
}

static int update_viewer(dx_mesh_viewer_scene* self, dx_i32 canvas_width, dx_i32 canvas_height) {
  return 0;
}

static int make_commands_1(dx_command_list* commands) {
  dx_command* command = NULL;

  // clear color command
  DX_VEC4 clear_color = { (dx_f32)dx_colors_capri.r / 255.f, (dx_f32)dx_colors_capri.g / 255.f, (dx_f32)dx_colors_capri.b / 255.f, 0.f };
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
  static dx_f32 const clear_depth = 1.f;
  command = dx_command_create_clear_depth(0, 0, 640, 480, clear_depth);
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

  return 0;
}

static int dx_mesh_viewer_scene_startup(dx_mesh_viewer_scene* self, dx_context* context) {
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
#if defined(DX_SAFE_MUL_IX_WITH_TESTS) && 1 == DX_SAFE_MUL_IX_WITH_TESTS
  if (dx_safe_mul_ix_tests()) {
    return 1;
  }
#endif
#if defined(DX_SAFE_MUL_NX_WITH_TESTS) && 1 == DX_SAFE_MUL_NX_WITH_TESTS
  if (dx_safe_mul_nx_tests()) {
    return 1;
  }
#endif
#if defined(DX_DDL_PARSER_WITH_TESTS) && 1 == DX_DDL_PARSER_WITH_TESTS
  if (dx_ddl_parser_tests()) {
    return 1;
  }
#endif
  {
    char* p;
    dx_size n;
    if (dx_get_file_contents(self->name, &p, &n)) {
      dx_memory_deallocate(p);
      p = NULL;
      return 1;
    }
    self->asset_scene = _create_scene_from_text(p, n);
    dx_memory_deallocate(p);
    p = NULL;
  }
  if(!self->asset_scene) {
    return 1;
  }
  //
  self->time = 0.f;
  //
  if (mesh_instance_on_startup(self, context, self->asset_scene)) {
    DX_UNREFERENCE(self->asset_scene);
    self->asset_scene = NULL;
    return 1;
  }
  //
  update_viewer(self, 640, 480);
  //
  {
    dx_command* command = NULL;
    dx_command_list* commands = dx_command_list_create();
    if (!commands) {
      mesh_instance_on_shutdown(self);
      DX_UNREFERENCE(self->asset_scene);
      self->asset_scene = NULL;
      return 1;
    }
    if (make_commands_1(commands)) {
      DX_UNREFERENCE(commands);
      commands = NULL;
      mesh_instance_on_shutdown(self);
      DX_UNREFERENCE(self->asset_scene);
      self->asset_scene = NULL;
      return 1;
    }
    self->commands = commands;
  }
  //
  return 0;
}

static int dx_mesh_viewer_scene_render(dx_mesh_viewer_scene* self, dx_context* context, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height) {
  update_viewer(self, canvas_width, canvas_height);
  
  DX_VEC4 current = { 0.f, 255.f / 255.f, 64.f / 255.f, 1.0f };

  {
    static const DX_VEC4 start = { 0.f, 255.f / 255.f, 64.f / 255.f, 1.0f }; // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
    static const DX_VEC4 end = { 255.f / 255.f, 192.f / 255.f, 0.f / 255.f }; // color called "Amber" (255, 192, 0) from "Capri"'s Split Complementary Palette.
    // fix the situation in which the sum might overflow
    if (isinf(self->time + delta_seconds)) {
      // this is effectively equivalen to
      // time = (time + available) % GREATEST if we
      dx_f32 available = (DX_F32_GREATEST - self->time);
      self->time = delta_seconds - available;
    } else {
      self->time += delta_seconds;
    }
    static dx_f32 const shift = 0.5f;
    static dx_f32 const amplitude = 0.5f;
    static dx_f32 const period = 8;
    static dx_f32 const DX_TWO_PI_F32 = DX_PI_F32 * 2.f;
    dx_f32 t = amplitude * sinf(self->time / period * DX_TWO_PI_F32) + shift;
    dx_vec4_lerp(&start, &end, t, &current);
  }
  {
    dx_command* command;

    // clear color command
    command = dx_command_list_get_at(self->commands, 0);
    command->clear_color_command.rectangle.w = (dx_f32)canvas_width;
    command->clear_color_command.rectangle.h = (dx_f32)canvas_height;

    // clear depth command
    command = dx_command_list_get_at(self->commands, 1);
    command->clear_depth_command.rectangle.w = (dx_f32)canvas_width;
    command->clear_depth_command.rectangle.h = (dx_f32)canvas_height;

    // viewport command
    command = dx_command_list_get_at(self->commands, 2);
    command->viewport_command.w = (dx_f32)canvas_width;
    command->viewport_command.h = (dx_f32)canvas_height;
  }
  // the "on enter frame" commands.
  if (dx_context_execute_commands(context, self->commands)) {
    return 1;
  }
  // the "per mesh instance" commands.
  {
    for (dx_size i = 0, n = dx_object_array_get_size(&self->mesh_instances); i < n; ++i) {
      dx_val_mesh_instance* mesh_instance = DX_VAL_MESH_INSTANCE(dx_object_array_get_at(&self->mesh_instances, i));
      if (!mesh_instance) {
        return 1;
      }
      // update the constant binding
      dx_cbinding* cbinding = dx_val_mesh_instance_get_cbinding(mesh_instance);
      mesh_instance->mesh->material->ambient_color = current;
      dx_val_mesh_instance_update_cbinding(mesh_instance, cbinding);
      viewer_push_constants(self, cbinding, canvas_width, canvas_height);
      if (dx_context_execute_commands(context, mesh_instance->commands)) {
        return 1;
      }
    }
  }
  return 0;
}

static int dx_mesh_viewer_scene_shutdown(dx_mesh_viewer_scene* self, dx_context* context) {
  mesh_instance_on_shutdown(self);
  if (self->asset_scene) {
    DX_UNREFERENCE(self->asset_scene);
    self->asset_scene = NULL;
  }
  if (self->commands) {
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
  }
  return 0;
}

int dx_mesh_viewer_scene_construct(dx_mesh_viewer_scene* self, char const* name) {
  dx_rti_type* _type = dx_mesh_viewer_scene_get_type();
  if (!_type) {
    return 1;
  }

  if (dx_scene_construct(DX_SCENE(self))) {
    return 1;
  }

  self->name = _strdup(name);
  if (!self->name) {
    return 1;
  }
  self->asset_scene = NULL;
  self->commands = NULL;

  DX_SCENE(self)->startup = (int (*)(dx_scene*, dx_context*)) & dx_mesh_viewer_scene_startup;
  DX_SCENE(self)->render = (int (*)(dx_scene*, dx_context*, dx_f32, dx_i32, dx_i32)) & dx_mesh_viewer_scene_render;
  DX_SCENE(self)->shutdown = (int (*)(dx_scene*, dx_context*)) dx_mesh_viewer_scene_shutdown;

  DX_OBJECT(self)->type = _type;

  return 0;
}

static void dx_mesh_viewer_scene_destruct(dx_mesh_viewer_scene* self) {
  if (self->name) {
    dx_memory_deallocate(self->name);
    self->name = NULL;
  }
}

dx_mesh_viewer_scene* dx_mesh_viewer_scene_create(char const* name) {
  dx_mesh_viewer_scene* self = DX_MESH_VIEWER_SCENE(dx_object_alloc(sizeof(dx_mesh_viewer_scene)));
  if (!self) {
    return NULL;
  }
  if (dx_mesh_viewer_scene_construct(self, name)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
