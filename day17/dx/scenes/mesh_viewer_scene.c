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
#include "dx/asset/material.h"
#include "dx/scenes/create_assets.h"

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


// the initial position of the viewer.
// must be different from TARGET.
static DX_VEC3 const SOURCE = { 0.f, 0.f, 2.f };

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

static int make_commands_1(dx_command_list* commands) {
  dx_command* command = NULL;

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
#if defined(DX_SAFE_MUL_NX_WITH_TESTS) && 1 == DX_SAFE_MUL_NX_WITH_TESTS
  if (dx_safe_mul_nx_tests()) {
    return 1;
  }
#endif
  {
    dx_asset_material* asset_material = _create_checkerboard_material();
    if (!asset_material) {
      return 1;
    }
    dx_asset_mesh* asset_mesh = _create_mesh(scene->name, asset_material);
    DX_UNREFERENCE(asset_material);
    asset_material = NULL;
    if (!asset_mesh) {
      return 1;
    }
    if (scene->on_mesh_loaded) {
      if (scene->on_mesh_loaded(asset_mesh)) {
        DX_UNREFERENCE(asset_mesh);
        asset_mesh = NULL;
        return 1;
      }
    }
    scene->mesh = dx_mesh_create(context, asset_mesh);
    if (!scene->mesh) {
      return 1;
    }
  }
  scene->angle = 0.f;

  //
  dx_mat4_set_identity(&scene->projection_matrix);
  update_viewer(scene);
  dx_mat4_set_identity(&scene->world_matrix);
  //
  {
    dx_command* command;
    dx_command_list* commands = dx_command_list_create();
    if (!commands) {
      DX_UNREFERENCE(scene->mesh);
      scene->mesh = NULL;
      return 1;
    }
    if (make_commands_1(commands)) {
      DX_UNREFERENCE(commands);
      commands = NULL;
      DX_UNREFERENCE(scene->mesh);
      scene->mesh = NULL;
      return 1;
    }
    dx_cbinding* cbinding = create_cbinding();
    if (!cbinding) {
      DX_UNREFERENCE(commands);
      commands = NULL;
      DX_UNREFERENCE(scene->mesh);
      scene->mesh = NULL;
      return 1;
    }
    // draw command
    command = dx_command_create_draw(scene->mesh->vbinding,
                                     scene->mesh->material,
                                     cbinding,
                                     scene->mesh->program,
                                     0,
                                     scene->mesh->asset_mesh->number_of_vertices);
    DX_UNREFERENCE(cbinding);
    cbinding = NULL;
    if (!command) {
      DX_UNREFERENCE(commands);
      commands = NULL;
      DX_UNREFERENCE(scene->mesh);
      scene->mesh = NULL;
      return 1;
    }
    if (dx_command_list_append(commands, command)) {
      DX_UNREFERENCE(command);
      command = NULL;
      DX_UNREFERENCE(commands);
      commands = NULL;
      DX_UNREFERENCE(scene->mesh);
      scene->mesh = NULL;
      return 1;
    }
    DX_UNREFERENCE(command);
    command = NULL;
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
  {
    // bind the uniforms
    dx_command* command = dx_command_list_get_at(scene->commands, 3);
    dx_cbinding* cbinding = command->draw_command.cbinding;
    //
    {
      static const DX_VEC4 mesh_color = { 0.f, 255.f / 255.f, 64.f / 255.f, 1.0f }; // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
      dx_cbinding_set_vec4(cbinding, "vs_mesh_ambient_rgba", &mesh_color);
      dx_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &scene->projection_matrix);
      dx_cbinding_set_mat4(cbinding, "matrices.view_matrix", &scene->view_matrix);
      dx_cbinding_set_mat4(cbinding, "vs_mesh_world_matrix", &scene->world_matrix);
    }
  }
  return dx_context_execute_commands(context, scene->commands);
}

static int dx_mesh_viewer_scene_shutdown(dx_mesh_viewer_scene* scene, dx_context* context) {
  if (scene->mesh) {
    DX_UNREFERENCE(scene->mesh);
    scene->mesh = NULL;
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
