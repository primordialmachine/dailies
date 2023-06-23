#include "dx/asset/scene.h"

#include "dx/asset/mesh_operations/set_vertex_colors.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void hashmap_on_added(dx_object** o);

static void hashmap_on_removed(dx_object** o);

static bool hashmap_on_compare_keys(dx_string** v, dx_string** w);

static dx_size hashmap_on_hash_key(dx_string** w);

static int on_initialize_mesh_operations(dx_pointer_hashmap* hashmap);

static void on_uninitialize_mesh_operations(dx_pointer_hashmap* hashmap);

static void hashmap_on_added(dx_object** o) {
  DX_REFERENCE(*o);
}

static void hashmap_on_removed(dx_object** o) {
  DX_UNREFERENCE(*o);
}

static bool hashmap_on_compare_keys(dx_string** v, dx_string** w) {
  return dx_string_is_equal_to(*v, *w);
}

static dx_size hashmap_on_hash_key(dx_string** w) {
  return dx_string_get_hash_value(*w);
}

static int on_initialize_mesh_operations(dx_pointer_hashmap *hashmap) {
  DX_POINTER_HASHMAP_CONFIGURATION configuration = {
    .compare_keys_callback = (dx_compare_keys_callback*) & hashmap_on_compare_keys,
    .hash_key_callback = (dx_hash_key_callback*) & hashmap_on_hash_key,
    .key_added_callback = (dx_key_added_callback*)&hashmap_on_added,
    .key_removed_callback = (dx_key_removed_callback*)&hashmap_on_removed,
    .value_added_callback = (dx_value_added_callback*) & hashmap_on_added,
    .value_removed_callback = (dx_value_removed_callback*) & hashmap_on_removed,
  };
  if (dx_pointer_hashmap_initialize(hashmap, &configuration)) {
    return 1;
  }
  {
    dx_string* name = dx_string_create("MeshOperations.SetVertexColors", sizeof("MeshOperations.SetVertexColors") - 1);
    if (!name) {
      dx_pointer_hashmap_uninitialize(hashmap);
      return 1;
    }
    dx_asset_mesh_operation* operation = DX_ASSET_MESH_OPERATION(dx_asset_mesh_operations_set_vertex_colors_create());
    if (!operation) {
      DX_UNREFERENCE(name);
      name = NULL;
      dx_pointer_hashmap_uninitialize(hashmap);
      return 1;
    }
    if (dx_pointer_hashmap_set(hashmap, name, operation)) {
      DX_UNREFERENCE(operation);
      operation = NULL;
      DX_UNREFERENCE(name);
      name = NULL;
      dx_pointer_hashmap_uninitialize(hashmap);
      return 1;
    }
    DX_UNREFERENCE(operation);
    operation = NULL;
    DX_UNREFERENCE(name);
    name = NULL;
  }
  return 0;
}

static void on_uninitialize_mesh_operations(dx_pointer_hashmap* hashmap) {
  dx_pointer_hashmap_uninitialize(hashmap);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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
  if (on_initialize_mesh_operations(&self->mesh_operations)) {
    DX_UNREFERENCE(self->palette);
    self->palette = NULL;
    dx_object_array_uninitialize(&self->mesh_instances);
    return 1;
  }
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_asset_scene_destruct;
  return 0;
}

void dx_asset_scene_destruct(dx_asset_scene* self) {
  on_uninitialize_mesh_operations(&self->mesh_operations);
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
