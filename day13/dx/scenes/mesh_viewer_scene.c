#include "dx/scenes/mesh_viewer_scene.h"

// _strdup
#include <string.h>
// sinf
#include <math.h>
// FLT_MAX
#include <float.h>
// snprintf
#include <stdio.h>
#include "dx/val/cbinding.h"
#include "dx/scenes/create_assets.h"
#include "dx/adl/syntactical.h"

DX_DEFINE_OBJECT_TYPE("dx.mesh_viewer_scene", dx_mesh_viewer_scene, dx_scene)

static int mesh_instance_on_startup(dx_mesh_viewer_scene* self, dx_context* context, dx_asset_scene* asset_scene) {
  if (dx_object_array_initialize(&self->mesh_instances, 2)) {
    return 1;
  }
  for (size_t i = 0, n = dx_object_array_get_size(&asset_scene->mesh_instances); i < n; ++i) {
    dx_asset_mesh_instance* asset_mesh_instance = DX_ASSET_MESH_INSTANCE(dx_object_array_get_at(&asset_scene->mesh_instances, i));
    dx_mesh* mesh = dx_mesh_create(context, asset_mesh_instance->mesh);
    if (!mesh) {
      return 1;
    }
    dx_mesh_instance* mesh_instance = dx_mesh_instance_create(asset_mesh_instance->world_matrix, mesh);
    if (dx_object_array_append(&self->mesh_instances, DX_OBJECT(mesh_instance))) {
      DX_UNREFERENCE(mesh_instance);
      mesh_instance = NULL;
      dx_object_array_uninitialize(&self->mesh_instances);
      return 1;
    }
    DX_UNREFERENCE(mesh_instance);
    mesh_instance = NULL;
  }
  return 0;
}

static void mesh_instance_on_shutdown(dx_mesh_viewer_scene* self) {
  dx_object_array_uninitialize(&self->mesh_instances);
}

static int viewer_push_constants(dx_mesh_viewer_scene* self, dx_cbinding* cbinding) {
  dx_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &self->projection_matrix);
  dx_cbinding_set_mat4(cbinding, "matrices.view_matrix", &self->view_matrix);
  return 0;
}

// the initial position of the viewer.
// must be different from TARGET.
static DX_VEC3 const SOURCE = { 0.f, 0.f, 1.f };

// Rotate viewer with 25 degrees per second counter-clockwise around the y-axis.
static float const degrees_per_second = 0.f;

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

static int make_commands_1(dx_command_list* commands) {
  dx_command* command = NULL;

  // clear color command
  DX_VEC4 clear_color = { (float)dx_colors_capri.r / 255.f, (float)dx_colors_capri.g / 255.f, (float)dx_colors_capri.b / 255.f, 0.f };
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
    if (!strcmp(scene->name, "quadriliteral")) {
      char* p;
      size_t n;
      if (dx_get_file_contents("./assets/quadriliteral.adl", &p, &n)) {
        dx_memory_deallocate(p);
        p = NULL;
        return 1;
      }
      scene->asset_scene = _create_scene_from_text(p, n);
      dx_memory_deallocate(p);
      p = NULL;
    } else if (!strcmp(scene->name, "triangle")) {
      char* p;
      size_t n;
      if (dx_get_file_contents("./assets/triangle.adl", &p, &n)) {
        dx_memory_deallocate(p);
        p = NULL;
        return 1;
      }
      scene->asset_scene = _create_scene_from_text(p, n);
      dx_memory_deallocate(p);
      p = NULL;
    }
  }
  if(!scene->asset_scene) {
    return 1;
  }
  //
  scene->angle = 0.f;
  if (mesh_instance_on_startup(scene, context, scene->asset_scene)) {
    DX_UNREFERENCE(scene->asset_scene);
    scene->asset_scene = NULL;
    return 1;
  }
  //
  dx_mat4_set_identity(&scene->projection_matrix);
  update_viewer(scene);
  //
  {
    dx_command* command = NULL;
    dx_command_list* commands = dx_command_list_create();
    if (!commands) {
      mesh_instance_on_shutdown(scene);
      DX_UNREFERENCE(scene->asset_scene);
      scene->asset_scene = NULL;
      return 1;
    }
    if (make_commands_1(commands)) {
      DX_UNREFERENCE(commands);
      commands = NULL;
      mesh_instance_on_shutdown(scene);
      DX_UNREFERENCE(scene->asset_scene);
      scene->asset_scene = NULL;
      return 1;
    }
    scene->commands = commands;
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

    // clear color command
    command = dx_command_list_get_at(scene->commands, 0);
    command->clear_color_command.rectangle.w = (float)canvas_width;
    command->clear_color_command.rectangle.h = (float)canvas_height;

    // clear depth command
    command = dx_command_list_get_at(scene->commands, 1);
    command->clear_depth_command.rectangle.w = (float)canvas_width;
    command->clear_depth_command.rectangle.h = (float)canvas_height;

    // viewport command
    command = dx_command_list_get_at(scene->commands, 2);
    command->viewport_command.w = (float)canvas_width;
    command->viewport_command.h = (float)canvas_height;
  }
  // the "on enter frame" commands.
  if (dx_context_execute_commands(context, scene->commands)) {
    return 1;
  }
  // the "per mesh instance" commands.
  {
    for (size_t i = 0, n = dx_object_array_get_size(&scene->mesh_instances); i < n; ++i) {
      dx_mesh_instance* mesh_instance = DX_MESH_INSTANCE(dx_object_array_get_at(&scene->mesh_instances, i));
      if (!mesh_instance) {
        return 1;
      }
      // update the constant binding
      dx_cbinding* cbinding = dx_mesh_instance_get_cbinding(mesh_instance);
      dx_mesh_instance_update_cbinding(mesh_instance, cbinding);
      viewer_push_constants(scene, cbinding);
      if (dx_context_execute_commands(context, mesh_instance->commands)) {
        return 1;
      }
    }
  }
  return 0;
}

static int dx_mesh_viewer_scene_shutdown(dx_mesh_viewer_scene* scene, dx_context* context) {
  mesh_instance_on_shutdown(scene);
  if (scene->asset_scene) {
    DX_UNREFERENCE(scene->asset_scene);
    scene->asset_scene = NULL;
  }
  if (scene->commands) {
    DX_UNREFERENCE(scene->commands);
    scene->commands = NULL;
  }
  dx_object_array_uninitialize(&scene->mesh_instances);
  return 0;
}

int dx_mesh_viewer_scene_construct(dx_mesh_viewer_scene* scene, char const* name) {
  dx_rti_type* _type = dx_mesh_viewer_scene_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_scene_construct(DX_SCENE(scene))) {
    return 1;
  }
  scene->name = _strdup(name);
  if (!scene->name) {
    return 1;
  }
  dx_mat4_set_identity(&scene->projection_matrix);
  dx_mat4_set_identity(&scene->view_matrix);
  dx_mat4_set_identity(&scene->projection_matrix);
  scene->asset_scene = NULL;
  scene->commands = NULL;
  DX_SCENE(scene)->startup = (int (*)(dx_scene*, dx_context*)) & dx_mesh_viewer_scene_startup;
  DX_SCENE(scene)->render = (int (*)(dx_scene*, dx_context*, float, int, int)) & dx_mesh_viewer_scene_render;
  DX_SCENE(scene)->shutdown = (int (*)(dx_scene*, dx_context*)) dx_mesh_viewer_scene_shutdown;
  DX_OBJECT(scene)->type = _type;
  return 0;
}

static void dx_mesh_viewer_scene_destruct(dx_mesh_viewer_scene* scene) {
  if (scene->name) {
    dx_memory_deallocate(scene->name);
    scene->name = NULL;
  }
}

dx_mesh_viewer_scene* dx_mesh_viewer_scene_create(char const* name) {
  dx_mesh_viewer_scene* scene = DX_MESH_VIEWER_SCENE(dx_object_alloc(sizeof(dx_mesh_viewer_scene)));
  if (!scene) {
    return NULL;
  }
  if (dx_mesh_viewer_scene_construct(scene, name)) {
    DX_UNREFERENCE(scene);
    scene = NULL;
    return NULL;
  }
  return scene;
}
