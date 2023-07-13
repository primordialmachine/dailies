// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com)
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#include "common.h"
#include "wic-plugin.h"


static int
write_image_1
  (
    wchar_t const* path,
    void const *source_bytes,
    DX_WIC_PLUGIN_PIXEL_FORMAT source_pixel_format,
    uint32_t source_stride,
    uint32_t source_width,
    uint32_t source_height,
    DX_WIC_PLUGIN_IMAGE_FORMAT target_format
  )
{
  int result = 0;

  IWICImagingFactory* piFactory = NULL;
  IWICStream* piStream = NULL;
  IWICBitmapEncoder* piEncoder = NULL;

  IWICBitmapFrameEncode* piBitmapFrame = NULL;
  IPropertyBag2* pPropertyBag = NULL;

  BYTE* pbBuffer = NULL;

  if (source_stride > UINT_MAX || source_width > UINT_MAX || source_height > UINT_MAX) {
    return 1;
  }

  HRESULT hr;
  //
  hr = CoInitialize(NULL);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  hr = CoCreateInstance(&CLSID_WICImagingFactory,
                        NULL,
                        CLSCTX_INPROC_SERVER,
                        &IID_IWICImagingFactory,
                        (LPVOID*)&piFactory);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  hr = IWICImagingFactory_CreateStream(piFactory, &piStream);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  hr = IWICStream_InitializeFromFilename(piStream, path, GENERIC_WRITE);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  GUID const* pformat = NULL;
  switch (target_format) {
    case DX_WIC_PLUGIN_IMAGE_FORMAT_TIFF:
      pformat = &GUID_ContainerFormatTiff;
      break;
    case DX_WIC_PLUGIN_IMAGE_FORMAT_PNG:
      pformat = &GUID_ContainerFormatPng;
      break;
    default: {
      result = 1;
      goto END;
    }
  }
  hr = IWICImagingFactory_CreateEncoder(piFactory, pformat, NULL, &piEncoder);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  hr = IWICBitmapEncoder_Initialize(piEncoder, (IStream*)piStream, WICBitmapEncoderNoCache);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  hr = IWICBitmapEncoder_CreateNewFrame(piEncoder, &piBitmapFrame, &pPropertyBag);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }

  // This is how you customize the TIFF output.
  switch (target_format) {
    // https://learn.microsoft.com/en-us/windows/win32/wic/tiff-format-overview
    case DX_WIC_PLUGIN_IMAGE_FORMAT_TIFF: {
      PROPBAG2 option = { 0 };
      option.pstrName = L"TiffCompressionMethod";
      VARIANT varValue;
      VariantInit(&varValue);
      varValue.vt = VT_UI1;
      varValue.bVal = WICTiffCompressionZIP;
      hr = IPropertyBag2_Write(pPropertyBag, 1, &option, &varValue);
      if (FAILED(hr)) {
        result = 1;
        goto END;
      }
    } break;
    // https://learn.microsoft.com/en-us/windows/win32/wic/png-format-overview
    case DX_WIC_PLUGIN_IMAGE_FORMAT_PNG: {
      /*Nothing to do here.*/
    } break;
    default: {
      result = 1;
      goto END;
    }
  };

  hr = IWICBitmapFrameEncode_Initialize(piBitmapFrame, pPropertyBag);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  hr = IWICBitmapFrameEncode_SetSize(piBitmapFrame, (UINT)source_width, (UINT)source_height);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  WICPixelFormatGUID formatGUID = GUID_WICPixelFormatUndefined;
  switch (source_pixel_format) {
    case DX_WIC_PLUGIN_PIXEL_FORMAT_RGB: {
      formatGUID = GUID_WICPixelFormat24bppBGR;
    } break;
    default: {
      result = 1;
      goto END;
    } break;
  };
  hr = IWICBitmapFrameEncode_SetPixelFormat(piBitmapFrame, &formatGUID);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  // Fail if the encoder cannot handle that pixel format.
  hr = IsEqualGUID(&formatGUID, &GUID_WICPixelFormat24bppBGR) ? S_OK : E_FAIL;
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  uint32_t buffer_size = source_stride* source_height;
  hr = IWICBitmapFrameEncode_WritePixels(piBitmapFrame, (UINT)source_height, source_stride, buffer_size, (void*)source_bytes);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }

  hr = IWICBitmapFrameEncode_Commit(piBitmapFrame);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }

  hr = IWICBitmapEncoder_Commit(piEncoder);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }

END:
  //
  if (pPropertyBag) {
    IPropertyBag2_Release(pPropertyBag);
    pPropertyBag = NULL;
  }
  if (piBitmapFrame) {
    IWICBitmapFrameEncode_Release(piBitmapFrame);
    piBitmapFrame = NULL;
  }
  //
  if (piEncoder) {
    IWICBitmapEncoder_Release(piEncoder);
    piEncoder = NULL;
  }
  //
  if (piStream) {
    IWICStream_Release(piStream);
    piStream = NULL;
  }
  //
  if (piFactory) {
    IWICImagingFactory_Release(piFactory);
    piFactory = NULL;
  }
  return result;
}

__declspec(dllexport) int
write_image
  (
    char const* path,
    void const* source_bytes,
    DX_WIC_PLUGIN_PIXEL_FORMAT source_pixel_format,
    uint32_t source_stride,
    uint32_t source_width,
    uint32_t source_height,
    DX_WIC_PLUGIN_IMAGE_FORMAT target_format
  )
{
  if (!path) {
    return 1;
  }
  wchar_t* path_1 = mb_to_wc(path);
  if (!path_1) {
    return 1;
  }
  if (write_image_1(path_1,
                    source_bytes,
                    source_pixel_format,
                    source_stride,
                    source_width,
                    source_height,
                    target_format)) {
    free(path_1);
    path_1 = NULL;
    return 1;
  }
  free(path_1);
  return 0;
}
