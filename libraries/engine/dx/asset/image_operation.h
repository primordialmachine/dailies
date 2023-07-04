#if !defined(DX_ASSET_IMAGE_OPERATION_H_INCLUDED)
#define DX_ASSET_IMAGE_OPERATION_H_INCLUDED

#include "dx/asset/color.h"

DX_DECLARE_OBJECT_TYPE("dx.asset.image_operation",
                       dx_asset_image_operation,
                       dx_object)

static inline dx_asset_image_operation* DX_ASSET_IMAGE_OPERATION(void* p) {
  return (dx_asset_image_operation*)p;
}

struct dx_asset_image_operation {
  dx_object _parent;
};

int dx_asset_image_operation_construct(dx_asset_image_operation* self);

#endif // DX_ASSET_IMAGE_OPERATION_H_INCLUDED
