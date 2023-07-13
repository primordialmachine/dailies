// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com)
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#include "common.h"
#include "wic-plugin.h"

static int
read_image_1
  (
    wchar_t const* path,
    DX_WIC_PLUGIN_IMAGE_FORMAT source_image_format,
    void** target_bytes, 
    DX_WIC_PLUGIN_PIXEL_FORMAT* target_pixel_format,
    uint32_t* target_stride,
    uint32_t* target_width,
    uint32_t* target_height,
    void* context,
    DX_WIC_PLUGIN_ALLOCATE_PROC* allocate,
    DX_WIC_PLUGIN_DEALLOCATE_PROC* deallocate
  )
{
  return 0;
}

__declspec(dllexport) int
read_image
  (
    char const* path,
    DX_WIC_PLUGIN_IMAGE_FORMAT source_image_format,
    void** target_bytes, 
    DX_WIC_PLUGIN_PIXEL_FORMAT* target_pixel_format,
    uint32_t* target_stride,
    uint32_t* target_width,
    uint32_t* target_height,
    void* context,
    DX_WIC_PLUGIN_ALLOCATE_PROC* allocate,
    DX_WIC_PLUGIN_DEALLOCATE_PROC* deallocate
  )
{
  if (!path) {
    return 1;
  }
  wchar_t* path_1 = mb_to_wc(path);
  if (!path_1) {
    return 1;
  }
  if (read_image_1(path_1,
                   source_image_format,
                   target_bytes,
                   target_pixel_format,
                   target_stride,
                   target_width,
                   target_height,
                   context,
                   allocate,
                   deallocate)) {
    free(path_1);
    path_1 = NULL;
    return 1;
  }
  free(path_1);
  return 0;
}
