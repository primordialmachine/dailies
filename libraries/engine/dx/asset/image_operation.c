#include "dx/asset/image_operation.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.image_operation",
                      dx_asset_image_operation,
                      dx_object)

static void dx_asset_image_operation_destruct(dx_asset_image_operation* self)
{/*Intentionally empty.*/}

int dx_asset_image_operation_construct(dx_asset_image_operation* self) {
  dx_rti_type* _type = dx_asset_image_operation_get_type();
  if (!_type) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}
