#include "dx/scenes/create_assets.h"

dx_asset_texture* _create_checkerbord_texture() {
  // Create a checkerboard texture.
  DX_ASSET_CHECKERBOARD_BRUSH brush;
  brush._parent.flags = DX_ASSET_BRUSH_FLAGS_CHECKBERBOARD;
  brush.number_of_checkers.horizontal = 16;
  brush.number_of_checkers.vertical = 16;
  brush.checker_size.horizontal = 128;
  brush.checker_size.vertical = 128;
  brush.checker_colors.first = dx_colors_malachite;
  brush.checker_colors.second = dx_colors_amber;
  struct {
    size_t horizontal;
    size_t vertical;
  } number_of_pixels = {
    .horizontal = brush.number_of_checkers.horizontal * brush.checker_size.horizontal,
    .vertical = brush.number_of_checkers.vertical * brush.checker_size.vertical,
  };
  dx_asset_image* asset_image = dx_asset_image_create_rgb_u8(DX_PIXEL_FORMAT_RGB_U8, number_of_pixels.horizontal, number_of_pixels.vertical, &dx_colors_malachite);
  if (!asset_image) {
    return NULL;
  }
  if (dx_asset_image_fill(asset_image, 0, 0, asset_image->width, asset_image->height, (DX_ASSET_BRUSH const*)&brush)) {
    DX_UNREFERENCE(asset_image);
    asset_image = NULL;
    return NULL;
  }
  dx_asset_texture* asset_texture = dx_asset_texture_create(asset_image);
  DX_UNREFERENCE(asset_image);
  asset_image = NULL;
  return asset_texture;
}

dx_asset_material* _create_checkerboard_material() {
  dx_asset_material* material = dx_asset_material_create();
  if (!material) {
    return NULL;
  }
  dx_asset_texture* texture = _create_checkerbord_texture();
  if (!texture) {
    DX_UNREFERENCE(material);
    material = NULL;
    return NULL;
  }
  if (dx_asset_material_set_ambient_texture(material, texture)) {
    DX_UNREFERENCE(material);
    material = NULL;
    DX_UNREFERENCE(texture);
    texture = NULL;
    return NULL;
  }
  DX_UNREFERENCE(texture);
  texture = NULL;
  return material;
}

dx_asset_mesh* _create_mesh(char const* specifier, dx_asset_material* material) {
  dx_asset_mesh* mesh = dx_asset_mesh_create(specifier, material);
  if (!mesh) {
    return NULL;
  }
  return mesh;
}
