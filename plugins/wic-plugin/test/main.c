// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com)
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <inttypes.h>
#define DLL_NAME "wic-plugin.dll"
#define WRITE_IMAGE_PROC_NAME "write_image"

/// @brief The source pixel format.
/// Three consecutive Bytes:
/// The first one denoting the intensity of the red component.
/// the second one denoting the intensity of the green component.
/// The third one denoting the intensity of the blue component.
#define DX_SOURCE_PIXEL_FORMAT_RGB (1)

/// @brief For writing: The target image format "tiff".
#define DX_TARGET_IMAGE_FORMAT_TIFF (1)
/// @brief For writing: The target image format "png". 
#define DX_TARGET_IMAGE_FORMAT_PNG (2)

/// @brief For reading: The source image format "tiff".
#define DX_SOURCE_IMAGE_FORMAT_TIFF (1)
/// @brief For reading: The source image format "png".
#define DX_SOURCE_IMAGE_FORMAT_PNG (2)

/// @param target_image_format The target image format. Must be one of
/// - DX_SOURCE_IMAGE_FORMAT_TIFF
/// - DX_SOURCE_IMAGE_FORMAT_PNG
typedef int (DX_WRITE_IMAGE_PROC)(char const* path, void const* source_bytes, uint8_t source_pixel_format, uint32_t stride, uint32_t source_width, uint32_t source_height, uint8_t target_image_format);

/// @param source_image_format The source image format. Must be one of
/// - DX_SOURCE_IMAGE_FORMAT_TIFF
/// - DX_SOURCE_IMAGE_FORMAT_PNG
typedef int (DX_READ_IMAGE_PROC)(char const* path, uint8_t source_image_format, void** bytes, void *context, void (*allocate)(void *context,size_t), void (*deallocate)(void *context, void* p));

static int test_tiff() {
  HMODULE hModule = LoadLibraryA("./" DLL_NAME);
  if (NULL == hModule) {
    return 1;
  }
  DX_WRITE_IMAGE_PROC* procedure = (DX_WRITE_IMAGE_PROC*)GetProcAddress(hModule, WRITE_IMAGE_PROC_NAME);
  if (!procedure) {
    FreeLibrary(hModule);
    hModule = NULL;
    return 1;
  }
  uint32_t source_stride = 256 * (24/8);
  uint32_t source_width = 256;
  uint32_t source_height = 256;
  uint8_t *buffer = malloc(sizeof(uint8_t) * source_stride * source_height);
  if (!buffer) {
    FreeLibrary(hModule);
    hModule = NULL;
    return 1;
  }
  for (size_t i = 0; i < sizeof(uint8_t) * source_stride * source_height; i++) {
    buffer[i] = (uint8_t)rand();
  }
  if (procedure("hello.tiff", buffer, DX_SOURCE_PIXEL_FORMAT_RGB, source_stride, source_width, source_height, DX_TARGET_IMAGE_FORMAT_TIFF)) {
    free(buffer);
    buffer = NULL;
    FreeLibrary(hModule);
    hModule = NULL;
    return 1;
  }
  free(buffer);
  buffer = NULL;
  FreeLibrary(hModule);
  hModule = NULL;
  return 0;
}

static int test_png() {
  HMODULE hModule = LoadLibraryA("./" DLL_NAME);
  if (NULL == hModule) {
    return 1;
  }
  DX_WRITE_IMAGE_PROC* procedure = (DX_WRITE_IMAGE_PROC*)GetProcAddress(hModule, WRITE_IMAGE_PROC_NAME);
  if (!procedure) {
    FreeLibrary(hModule);
    hModule = NULL;
    return 1;
  }
  uint32_t source_stride = 256 * (24 / 8);
  uint32_t source_width = 256;
  uint32_t source_height = 256;
  uint8_t* buffer = malloc(sizeof(uint8_t) * source_stride * source_height);
  if (!buffer) {
    FreeLibrary(hModule);
    hModule = NULL;
    return 1;
  }
  for (size_t i = 0; i < sizeof(uint8_t) * source_stride * source_height; i++) {
    buffer[i] = (uint8_t)rand();
  }
  if (procedure("hello.png", buffer, DX_SOURCE_PIXEL_FORMAT_RGB, source_stride, source_width, source_height, DX_TARGET_IMAGE_FORMAT_PNG)) {
    free(buffer);
    buffer = NULL;
    FreeLibrary(hModule);
    hModule = NULL;
    return 1;
  }
  free(buffer);
  buffer = NULL;
  FreeLibrary(hModule);
  hModule = NULL;
  return 0;
}

int main(int argc, char** argv) {
  if (test_tiff() || test_png()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
