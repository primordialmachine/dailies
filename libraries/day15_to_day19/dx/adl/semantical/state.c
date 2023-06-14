#include "dx/adl/semantical/state.h"

#include "dx/adl/syntactical.h"

static bool compare_keys_callback(dx_string** key1, dx_string** key2);

static size_t hash_key_callback(dx_string** key);

static void key_added_callback(dx_string** key);

static void key_removed_callback(dx_string** key);

static void value_added_callback(dx_adl_node** value);

static void value_removed_callback(dx_adl_node** value);

/// @brief Create the default palette.
/// @return A pointer to the palette on success. The null pointer on failure.
static dx_asset_palette* _create_default_palette();

static bool compare_keys_callback(dx_string** key1, dx_string** key2) {
  DX_DEBUG_ASSERT(NULL != key1);
  DX_DEBUG_ASSERT(NULL != *key1);
  DX_DEBUG_ASSERT(NULL != key2);
  DX_DEBUG_ASSERT(NULL != *key2);
  return dx_string_is_equal_to(*key1, *key2);
}

static size_t hash_key_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  return dx_string_get_hash_value(*key);
}

static void key_added_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_REFERENCE(*key);
}

static void key_removed_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_UNREFERENCE(*key);
}

static void value_added_callback(dx_adl_node** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_REFERENCE(*value);
}

static void value_removed_callback(dx_adl_node** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_UNREFERENCE(*value);
}

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

int dx_adl_semantical_state_construct(dx_adl_semantical_state* self) {
  self->palette = _create_default_palette();
  if (!self->palette) {
    return 1;
  }
  static DX_POINTER_HASHMAP_CONFIGURATION const configuration = {
    .compare_keys_callback = (dx_compare_keys_callback*)&compare_keys_callback,
    .hash_key_callback = (dx_hash_key_callback*)&hash_key_callback,
    .key_added_callback = (dx_key_added_callback*)&key_added_callback,
    .key_removed_callback = (dx_key_removed_callback*)&key_removed_callback,
    .value_added_callback = (dx_value_added_callback*)&value_added_callback,
    .value_removed_callback = (dx_value_removed_callback*)&value_removed_callback,
  };
  if (dx_pointer_hashmap_initialize(&self->named_nodes, &configuration)) {
    DX_UNREFERENCE(self->palette);
    self->palette = NULL;
    return 1;
  }
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_adl_semantical_state_destruct;
  return 0;
}

void dx_adl_semantical_state_destruct(dx_adl_semantical_state* self) {
  if (self->palette) {
    DX_UNREFERENCE(self->palette);
    self->palette = NULL;
  }
  dx_pointer_hashmap_uninitialize(&self->named_nodes);
}

dx_adl_semantical_state* dx_adl_semantical_state_create() {
  dx_adl_semantical_state* self = DX_ADL_SEMANTICAL_STATE(dx_object_alloc(sizeof(dx_adl_semantical_state)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_state_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_adl_semantical_add_named_node(dx_adl_semantical_state* self, dx_string* name, dx_adl_node* node) {
  if (!self || !name || !node) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  if (dx_pointer_hashmap_set(&self->named_nodes, (dx_pointer_hashmap_key)name, (dx_pointer_hashmap_value)node)) {
    return 1;
  }
  return 0;
}
