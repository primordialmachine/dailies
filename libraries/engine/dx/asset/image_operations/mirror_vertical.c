#include "dx/asset/image_operations/mirror_vertical.h"


DX_DEFINE_OBJECT_TYPE("dx.asset.image_operations.mirror_vertical",
                      dx_asset_image_operations_mirror_vertical,
                      dx_asset_image_operation)

static void dx_asset_image_operations_mirror_vertical_destruct(dx_asset_image_operations_mirror_vertical* self)
{/*Intentionally empty.*/}

int dx_asset_image_operations_mirror_vertical_construct(dx_asset_image_operations_mirror_vertical* self) {
  dx_rti_type* _type = dx_asset_image_operations_mirror_vertical_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_asset_image_operation_construct(DX_ASSET_IMAGE_OPERATION(self))) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_image_operations_mirror_vertical* dx_asset_image_operations_mirror_vertical_create() {
  dx_rti_type* _type = dx_asset_image_operations_mirror_vertical_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_image_operations_mirror_vertical* self = DX_ASSET_IMAGE_OPERATIONS_MIRROR_VERTICAL(dx_object_alloc(sizeof(dx_asset_image_operations_mirror_vertical)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_image_operations_mirror_vertical_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
