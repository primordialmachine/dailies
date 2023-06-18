#include "dx/asset/palette.h"

#include <malloc.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.palette_entry", dx_asset_palette_entry, dx_object)

int dx_asset_palette_entry_construct(dx_asset_palette_entry* self, dx_string* name, DX_RGB_U8 const* value) {
  if (!self || !name || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  self->name = name;
  DX_REFERENCE(self->name);
  self->value = *value;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_asset_palette_entry_destruct;
  return 0;
}

void dx_asset_palette_entry_destruct(dx_asset_palette_entry* self) {
  DX_UNREFERENCE(self->name);
  self->name = NULL;
}

dx_asset_palette_entry* dx_asset_palette_entry_create(dx_string* name, DX_RGB_U8 const* value) {
  dx_asset_palette_entry* self = DX_ASSET_PALETTE_ENTRY(dx_object_alloc(sizeof(dx_asset_palette_entry)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_palette_entry_construct(self, name, value)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self; 
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.palette", dx_asset_palette, dx_object)

static void on_added(dx_object** a);

static void on_removed(dx_object** a);

static size_t on_hash_key(dx_object** a);

static bool on_compare_keys(dx_object** a, dx_object** b);

static void on_added(dx_object** a) {
  DX_DEBUG_CHECK_MAGIC_BYTES(*a);
  DX_REFERENCE(*a);
}

static void on_removed(dx_object** a) {
  DX_DEBUG_CHECK_MAGIC_BYTES(*a);
  DX_UNREFERENCE(*a);
}

static size_t on_hash_key(dx_object** a) {
  DX_DEBUG_CHECK_MAGIC_BYTES(*a);
  return dx_string_get_hash_value(DX_STRING(*a));
}

static bool on_compare_keys(dx_object** a, dx_object** b) {
  DX_DEBUG_CHECK_MAGIC_BYTES(*a);
  DX_DEBUG_CHECK_MAGIC_BYTES(*b);
  return dx_string_is_equal_to(DX_STRING(*a), DX_STRING(*b));
}

int dx_asset_palette_construct(dx_asset_palette* self) {
  if (!self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  static DX_POINTER_HASHMAP_CONFIGURATION const configuration = {
    .compare_keys_callback = (bool(*)(void**,void**)) & on_compare_keys,
    .hash_key_callback = (size_t(*)(void**)) & on_hash_key,
    .key_added_callback = (void(*)(void**)) & on_added,
    .key_removed_callback = (void(*)(void**)) & on_removed,
    .value_added_callback = (void(*)(void**)) & on_added,
    .value_removed_callback = (void(*)(void**)) & on_removed,
  };
  if (dx_pointer_hashmap_initialize(&self->map, &configuration)) {
    return 1;
  }
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_asset_palette_destruct;
  return 0;
}

void dx_asset_palette_destruct(dx_asset_palette* self) {
  dx_pointer_hashmap_uninitialize(&self->map);
}

dx_asset_palette* dx_asset_palette_create() {
  dx_asset_palette* self = DX_ASSET_PALETTE(dx_object_alloc(sizeof(dx_asset_palette)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_palette_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

dx_asset_palette_entry* dx_asset_palette_get(dx_asset_palette const* self, dx_string* name) {
  if (!self || !name) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  return dx_pointer_hashmap_get(&self->map, name);
}

int dx_asset_palette_set(dx_asset_palette* self, dx_string* name, DX_RGB_U8 const* value) {
  if (!self || !name || !value) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_asset_palette_entry* entry = dx_asset_palette_entry_create(name, value);
  if (!entry) {
    return 1;
  }
  if (dx_pointer_hashmap_set(&self->map, entry->name, entry)) {
    DX_UNREFERENCE(entry);
    entry = NULL;
    return 1;
  }
  DX_UNREFERENCE(entry);
  entry = NULL;
  return 0;
}
