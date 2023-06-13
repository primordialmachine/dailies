#if !defined(DX_ASSET_PALETTE_H_INCLUDED)
#define DX_ASSET_PALETTE_H_INCLUDED

#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct dx_asset_palette_entry dx_asset_palette_entry;
static inline dx_asset_palette_entry* DX_ASSET_PALETTE_ENTRY(void* p) {
  return (dx_asset_palette_entry*)p;
}

struct dx_asset_palette_entry {
  dx_object _parent;
  dx_string* name;
  DX_RGB_U8 value;
};

int dx_asset_palette_entry_construct(dx_asset_palette_entry* self, dx_string* name, DX_RGB_U8 const* value);

void dx_asset_palette_entry_destruct(dx_asset_palette_entry* self);

dx_asset_palette_entry* dx_asset_palette_entry_create(dx_string* name, DX_RGB_U8 const* value);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct dx_asset_palette dx_asset_palette;
static inline dx_asset_palette* DX_ASSET_PALETTE(void* p) {
  return (dx_asset_palette*)p;
}

struct dx_asset_palette {
  dx_object _parent;
  /// A map from name object to color objects.
  /// More than one name can map to the same color object.
  dx_pointer_hashmap map;
};

int dx_asset_palette_construct(dx_asset_palette* self);

void dx_asset_palette_destruct(dx_asset_palette* self);

dx_asset_palette* dx_asset_palette_create();

dx_asset_palette_entry* dx_asset_palette_get(dx_asset_palette const* self, dx_string* name);

int dx_asset_palette_set(dx_asset_palette* self, dx_string* name, DX_RGB_U8 const* color);

#endif // DX_ASSET_PALETTE_H_INCLUDED
