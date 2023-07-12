#include "dx/asset/image_operations/mirror_horizontal.h"


DX_DEFINE_OBJECT_TYPE("dx.asset.image_operations.mirror_horizontal",
                      dx_asset_image_operations_mirror_horizontal,
                      dx_asset_image_operation)

static void dx_asset_image_operations_mirror_horizontal_destruct(dx_asset_image_operations_mirror_horizontal* self)
{/*Intentionally empty.*/}

int dx_asset_image_operations_mirror_horizontal_construct(dx_asset_image_operations_mirror_horizontal* self) {
  dx_rti_type* _type = dx_asset_image_operations_mirror_horizontal_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_asset_image_operation_construct(DX_ASSET_IMAGE_OPERATION(self))) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_image_operations_mirror_horizontal* dx_asset_image_operations_mirror_horizontal_create() {
  dx_rti_type* _type = dx_asset_image_operations_mirror_horizontal_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_image_operations_mirror_horizontal* self = DX_ASSET_IMAGE_OPERATIONS_MIRROR_HORIZONTAL(dx_object_alloc(sizeof(dx_asset_image_operations_mirror_horizontal)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_image_operations_mirror_horizontal_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
