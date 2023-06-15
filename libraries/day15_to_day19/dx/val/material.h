#if !defined(DX_MATERIAL_H_INCLUDED)
#define DX_MATERIAL_H_INCLUDED

#include "dx/val/context.h"
#include "dx/asset/material.h"

/// @brief The backend-representation of a material.
/// A dx.material usually represents a dx.asset.material.
DX_DECLARE_OBJECT_TYPE("dx.material", dx_material, dx_object)

static inline dx_material* DX_MATERIAL(void* p) {
  return (dx_material*)p;
}

struct dx_material {
  dx_object _parent;
  dx_context* context;
  /// @brief A pointer to the underlaying material asset.
  dx_asset_material* asset_material;
  /// @brief The ambient color as specified by dx_asset_material::ambient_color.
  DX_VEC4 ambient_color;
  /// @brief Pointer to the ambient texture or the null pointer as specified by_dx_asset_material::ambient_texture.
  dx_texture* ambient_texture;
};

int dx_material_construct(dx_material* self, dx_context* context, dx_asset_material* asset_material);

void dx_material_destruct(dx_material* self);

dx_material* dx_material_create(dx_context* context, dx_asset_material* asset_material);

#endif // DX_MATERIAL_H_INCLUDED
