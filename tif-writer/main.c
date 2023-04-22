// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com)
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

// malloc, free
#include <malloc.h>

#define COBJMACROS (1)
#include <wincodec.h>
#pragma comment (lib, "Windowscodecs.lib")

static wchar_t* mb_to_wc(const char* source) {
  int result;
  result = MultiByteToWideChar(CP_ACP, 0, source, -1, NULL, 0);
  if (!result) {
    return NULL;
  }
  wchar_t* target = malloc(sizeof(wchar_t) * result);
  if (!target) {
    return NULL;
  }
  result = MultiByteToWideChar(CP_ACP, 0, source, -1, target, result);
  if (!result) {
    free(target);
    target = NULL;
    return NULL;
  }
  return target;
}

static int write_image_1(wchar_t const* path, int width, int height, int format) {
  int result = 0;

  IWICImagingFactory* piFactory = NULL;
  IWICStream* piStream = NULL;
  IWICBitmapEncoder* piEncoder = NULL;

  IWICBitmapFrameEncode* piBitmapFrame = NULL;
  IPropertyBag2* pPropertyBag = NULL;

  BYTE* pbBuffer = NULL;

  if (width < 0 || width > UINT_MAX || height < 0 || height > UINT_MAX) {
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
  hr = IWICImagingFactory_CreateEncoder(piFactory, &GUID_ContainerFormatTiff, NULL, &piEncoder);
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
  hr = IWICBitmapFrameEncode_Initialize(piBitmapFrame, pPropertyBag);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  hr = IWICBitmapFrameEncode_SetSize(piBitmapFrame, (UINT)width, (UINT)height);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  WICPixelFormatGUID formatGUID = GUID_WICPixelFormat24bppBGR;
  hr = IWICBitmapFrameEncode_SetPixelFormat(piBitmapFrame, &formatGUID);
  if (hr) {
    result = 1;
    goto END;
  }
  // Fail if the encoder cannot handle that pixel format.
  hr = IsEqualGUID(&formatGUID, &GUID_WICPixelFormat24bppBGR) ? S_OK : E_FAIL;
  if (hr) {
    result = 1;
    goto END;
  }

  UINT cbStride = (((UINT)width) * 24 + 7) / 8/***WICGetStride***/;
  UINT cbBufferSize = ((UINT)height) * cbStride;

  pbBuffer = malloc(sizeof(BYTE) * cbBufferSize);
  if (!pbBuffer) {
    result = 1;
    goto END;
  }

  for (UINT i = 0; i < cbBufferSize; i++)
  {
    pbBuffer[i] = (BYTE)rand();
  }

  hr = IWICBitmapFrameEncode_WritePixels(piBitmapFrame, (UINT)height, cbStride, cbBufferSize, pbBuffer);
  if (hr) {
    result = 1;
    goto END;
  }

  hr = IWICBitmapFrameEncode_Commit(piBitmapFrame);
  if (hr) {
    result = 1;
    goto END;
  }

  hr = IWICBitmapEncoder_Commit(piEncoder);
  if (hr) {
    result = 1;
    goto END;
  }

END:
  if (pbBuffer) {
    free(pbBuffer);
    pbBuffer = NULL;
  }
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

__declspec(dllexport) int write_image(char const* path, int width, int height, int format) {
  if (!path || width < 0 || height < 0) {
    return 1;
  }
  wchar_t* path_1 = mb_to_wc(path);
  if (!path_1) {
    return 1;
  }
  if (write_image_1(path_1, width, height, format)) {
    free(path_1);
    path_1 = NULL;
    return 1;
  }
  free(path_1);
  return 0;
}
