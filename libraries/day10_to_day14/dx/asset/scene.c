#include "dx/asset/scene.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.scene", dx_asset_scene, dx_object)

int dx_asset_scene_construct(dx_asset_scene* self) {
  if (dx_object_array_initialize(&self->mesh_instances, 0)) {
      return 1;
  }
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_scene_destruct;
  return 0;
}

void dx_asset_scene_destruct(dx_asset_scene* self) {
  dx_object_array_uninitialize(&self->mesh_instances);
}

dx_asset_scene* dx_asset_scene_create() {
  dx_asset_scene* self = DX_ASSET_SCENE(dx_object_alloc(sizeof(dx_asset_scene)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_scene_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
