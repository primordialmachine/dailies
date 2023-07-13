#if !defined(DX_WIC_PLUGIN_H_INCLUDED)
#define DX_WIC_PLUGIN_H_INCLUDED

#include <inttypes.h>
#define DX_WIC_PLUGIN_LIBRARY_NAME "wic-plugin"

/// @brief DX_WC_PLUGIN_PIXEL_FORMAT value.
/// Denotes the following pixel format:
/// Three consecutive Bytes.
/// The first one denoting the intensity of the red component.
/// the second one denoting the intensity of the green component.
/// The third one denoting the intensity of the blue component.
#define DX_WIC_PLUGIN_PIXEL_FORMAT_RGB (1)

/// @brief Type of values denoting pixel formats.
typedef uint8_t DX_WIC_PLUGIN_PIXEL_FORMAT;

/// @brief
/// DX_WIC_PLUGIN_IMAGE_FORMAT value.
/// Denotes the "TIFF" image format.
#define DX_WIC_PLUGIN_IMAGE_FORMAT_TIFF (1)

/// @brief
/// DX_WIC_PLUGIN_IMAGE_FORMAT value.
/// Denotes the "PNG" image format.
#define DX_WIC_PLUGIN_IMAGE_FORMAT_PNG (2)

/// @brief Type of values denoting image formats.
typedef uint8_t DX_WIC_PLUGIN_IMAGE_FORMAT;

/// @brief The procedure name of the "write image" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_WIC_PLUGIN_WRITE_IMAGE_PROC_NAME "write_image"

/// @param target_image_format The target image format. Must be one of
/// - DX_WIC_PLUGIN_SOURCE_IMAGE_FORMAT_TIFF
/// - DX_WIC_PLUGIN_SOURCE_IMAGE_FORMAT_PNG
typedef int (DX_WIC_PLUGIN_WRITE_IMAGE_PROC)(char const* path,
                                             void const* source_bytes,
                                             DX_WIC_PLUGIN_PIXEL_FORMAT source_pixel_format,
                                             uint32_t source_stride,
                                             uint32_t source_width,
                                             uint32_t source_height,
                                             DX_WIC_PLUGIN_IMAGE_FORMAT target_image_format);

/// @brief The procedure name of the "write image" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_WIC_PLUGIN_READ_IMAGE_PROC_NAME "read_image"

typedef void *(DX_WIC_PLUGIN_ALLOCATE_PROC)(void* context, size_t number_of_bytes);
typedef void (DX_WIC_PLUGIN_DEALLOCATE_PROC)(void* context, void* bytes);

/// @param source_image_format The source image format. Must be one of
/// - DX_WIC_PLUGIN_SOURCE_IMAGE_FORMAT_TIFF
/// - DX_WIC_PLUGIN_SOURCE_IMAGE_FORMAT_PNG
typedef int (DX_WIC_PLUGIN_READ_IMAGE_PROC)(char const* path,
                                            DX_WIC_PLUGIN_IMAGE_FORMAT source_image_format,
                                            void** target_bytes,
                                            DX_WIC_PLUGIN_PIXEL_FORMAT *target_pixel_format,
                                            uint32_t *target_stride,
                                            uint32_t *target_width,
                                            uint32_t* target_height,
                                            void *context,
                                            DX_WIC_PLUGIN_ALLOCATE_PROC* allocate,
                                            DX_WIC_PLUGIN_DEALLOCATE_PROC* deallocate);

#endif // DX_WIC_PLUGIN_H_INCLUDED
