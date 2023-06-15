#if !defined(DX_ASSET_MATERIAL_H_INCLUDED)
#define DX_ASSET_MATERIAL_H_INCLUDED

#include "dx/asset/texture.h"

/// @brief A material asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.material", dx_asset_material, dx_object)

static inline dx_asset_material* DX_ASSET_MATERIAL(void* p) {
    return (dx_asset_material*)p;
}

struct dx_asset_material {
  dx_object _parent;
  /// @brief The ambient color of this material.
  /// @default The color "opaque white" / "(1,1,1,1)".
  DX_VEC4 ambient_color;
  /// @brief Pointer to the ambient texture of this material or the null pointer.
  /// @default The null pointer.
  dx_asset_texture* ambient_texture;
};

/// @brief Construct this dx_asset_material object with default values.
/// @param self A pointer to this dx_asset_material object.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int dx_asset_material_construct(dx_asset_material* self);

/// @brief Destruct this dx_asset_material object.
/// @param self A pointer to this dx_asset_material object.
/// @undefined @a self does not point to a constructed dx_asset_material object.
void dx_asset_material_destruct(dx_asset_material* self);

/// @brief Create a dx_asset_material object with default values.
/// @return A pointer to the dx_asset_material object on success. The null pointer failure.
/// @failure This function has set the error variable.
/// - #DX_ALLOCATION_FAILED an allocation failed
dx_asset_material* dx_asset_material_create();

/// @brief Set the ambient color.
/// @param self A pointer to this dx_asset_material object.
/// @param color A pointer to color.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
int dx_asset_material_set_ambient_color(dx_asset_material* self, DX_VEC4 const* value);

/// @brief Set the ambient texture.
/// @param self A pointer to this dx_asset_material object.
/// @param texture A pointer to the texture or the null pointer.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable.
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
int dx_asset_material_set_ambient_texture(dx_asset_material* self, dx_asset_texture* value);

#endif // DX_ASSET_MATERIAL_H_INCLUDED
