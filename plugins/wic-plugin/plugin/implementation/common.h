// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com)
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#if !defined(WIC_PLUGIN_COMMON_H_INCLUDED)
#define WIC_PLUGIN_COMMON_H_INCLUDED

// uint8_t, uint32_t
#include <inttypes.h>

// malloc, free
#include <malloc.h>

// WIC header
#define COBJMACROS (1)
#include <wincodec.h>
// WIC library
#pragma comment (lib, "Windowscodecs.lib")

wchar_t* mb_to_wc(const char* source);

#endif // WIC_PLUGIN_COMMON_H_INCLUDED
