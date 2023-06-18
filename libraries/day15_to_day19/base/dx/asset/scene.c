#include "dx/asset/scene.h"

/// @internal
/// @brief Create the default palette.
/// @return A pointer to the palette on success. The null pointer on failure.
static dx_asset_palette* _create_default_palette();

static dx_asset_palette* _create_default_palette() {
  dx_asset_palette* palette = dx_asset_palette_create();
  if (!palette) {
    return NULL;
  }
#define DEFINE(NAME, R, G, B) \
  { \
    dx_string* name = dx_string_create(NAME, sizeof(NAME) - 1); \
    if (!name) { \
      DX_UNREFERENCE(palette); \
      palette = NULL; \
      return NULL; \
    } \
    DX_RGB_U8 value = { .r = R, .g = G, .b = B }; \
    if (dx_asset_palette_set(palette, name, &value)) { \
      DX_UNREFERENCE(name); \
      name = NULL; \
      DX_UNREFERENCE(palette); \
      palette = NULL; \
      return NULL; \
    } \
  }

  DEFINE("malachite", dx_colors_malachite.r, dx_colors_malachite.g, dx_colors_malachite.b);
  DEFINE("amber", dx_colors_amber.r, dx_colors_amber.g, dx_colors_amber.b);
  DEFINE("capri", dx_colors_capri.r, dx_colors_capri.g, dx_colors_capri.b);

#undef DEFINE

  return palette;
}

DX_DEFINE_OBJECT_TYPE("dx.asset.scene", dx_asset_scene, dx_object)

int dx_asset_scene_construct(dx_asset_scene* self) {
  if (dx_object_array_initialize(&self->mesh_instances, 0)) {
      return 1;
  }
  self->palette = _create_default_palette();
  if (!self->palette) {
    dx_object_array_uninitialize(&self->mesh_instances);
    return 1;
  }
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_scene_destruct;
  return 0;
}

void dx_asset_scene_destruct(dx_asset_scene* self) {
  if (self->palette) {
    DX_UNREFERENCE(self->palette);
    self->palette = NULL;
  }
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
