#include "dx/scene.h"

#include <malloc.h>

DX_DEFINE_OBJECT_TYPE("dx.scene", dx_scene, dx_object)

int dx_scene_construct(dx_scene* scene) {
  scene->startup = NULL;
  scene->render = NULL;
  scene->shutdown = NULL;
  DX_OBJECT(scene)->destruct = (void(*)(dx_object*)) & dx_scene_destruct;
  return 0;
}

void dx_scene_destruct(dx_scene* scene) { }

int dx_scene_startup(dx_scene* scene, dx_context* context) {
  if (!scene) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (!scene->startup) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return scene->startup(scene, context);
}

int dx_scene_render(dx_scene* scene, dx_context* context, float delta_seconds, int canvas_width, int canvas_height) {
  if (!scene) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (!scene->render) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return scene->render(scene, context, delta_seconds, canvas_width, canvas_height);
}

int dx_scene_shutdown(dx_scene* scene, dx_context* context) {
  if (!scene) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (!scene->shutdown) {
    dx_set_error(DX_INVALID_OPERATION);
    return 1;
  }
  return scene->shutdown(scene, context);
}
