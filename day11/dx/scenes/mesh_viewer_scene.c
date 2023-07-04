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
#include "dx/adl/syntactical.h"
#include "dx/asset/optics.h"

DX_DEFINE_OBJECT_TYPE("dx.mesh_viewer_scene",
                      dx_mesh_viewer_scene,
                      dx_scene)

static int on_scene_asset_object(dx_mesh_viewer_scene* self, dx_context* context, dx_object* asset_object) {
  // mesh instance
  if (dx_rti_type_is_leq(asset_object->type, dx_asset_mesh_instance_get_type())) {
    dx_asset_mesh_instance* asset_mesh_instance = DX_ASSET_MESH_INSTANCE(asset_object);
    dx_mesh* mesh = dx_mesh_create(context, asset_mesh_instance->mesh);
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

static int viewer_push_constants(dx_mesh_viewer_scene* self, dx_cbinding* cbinding) {
  dx_val_viewer* viewer = DX_VAL_VIEWER(dx_object_array_get_at(&self->viewers, dx_object_array_get_size(&self->viewers) - 1));
  if (dx_get_error()) {
    return 1;
  }
  dx_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &self->projection_matrix);
  dx_cbinding_set_mat4(cbinding, "matrices.view_matrix", &viewer->view_matrix);
  return 0;
}

// The initial position of the viewer.
// Must be different from TARGET.
static DX_VEC3 const SOURCE = { 0.f, 0.f, 1.f };

// Rotate viewer with 0 degrees per second counter-clockwise around the y-axis.
static dx_f32 const degrees_per_second = 0.f;

static int update_viewer(dx_mesh_viewer_scene* self) {
  dx_val_viewer* viewer = DX_VAL_VIEWER(dx_object_array_get_at(&self->viewers, dx_object_array_get_size(&self->viewers) - 1));
  if (dx_get_error()) {
    return 1;
  }
  DX_MAT4 a;
  dx_mat4_set_rotate_y(&a, self->angle);
  dx_transform_point(&viewer->source, &SOURCE, &a);
  dx_mat4_set_look_at(&viewer->view_matrix, &viewer->source, &viewer->target, &viewer->up);
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
#if defined(DX_ADL_PARSER_WITH_TESTS) && 1 == DX_ADL_PARSER_WITH_TESTS
  if (dx_adl_parser_tests()) {
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
  self->angle = 0.f;
  if (mesh_instance_on_startup(self, context, self->asset_scene)) {
    DX_UNREFERENCE(self->asset_scene);
    self->asset_scene = NULL;
    return 1;
  }
  //
  dx_mat4_set_identity(&self->projection_matrix);
  update_viewer(self);
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
  dx_asset_optics* optics = DX_ASSET_OPTICS(dx_asset_optics_orthographic_create());
  if (!optics) {
    return 1;
  }
  if (dx_rti_type_is_leq(DX_OBJECT(optics)->type, dx_asset_optics_perspective_get_type())) {
    dx_asset_optics_perspective* optics1 = DX_ASSET_OPTICS_PERSPECTIVE(optics);
    dx_mat4_set_perspective(&self->projection_matrix, 60.f, (dx_f32)canvas_width / (dx_f32)canvas_height,
                            optics1->near, optics1->far);
  } else if (dx_rti_type_is_leq(DX_OBJECT(optics)->type, dx_asset_optics_orthographic_get_type())) {
    dx_asset_optics_orthographic* optics1 = DX_ASSET_OPTICS_ORTHOGRAPHIC(optics);
    dx_mat4_set_ortho(&self->projection_matrix, -1, +1, -1, +1, optics1->near, optics1->far);
  } else {
    DX_UNREFERENCE(optics);
    optics = NULL;
    return 1;
  }

  DX_UNREFERENCE(optics);
  optics = NULL;
  
  update_viewer(self);

  dx_f32 degrees = degrees_per_second * delta_seconds;
  self->angle = fmodf(self->angle + degrees, 360.f);
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
      dx_val_mesh_instance_update_cbinding(mesh_instance, cbinding);
      viewer_push_constants(self, cbinding);
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
  dx_mat4_set_identity(&self->projection_matrix);
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
