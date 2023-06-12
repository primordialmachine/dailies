/// @file dx/core.h
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @brief Main include file for core functionality.
#if !defined(DX_CORE_H_INCLUDED)
#define DX_CORE_H_INCLUDED

/// @defgroup core General-purpose, re-usable, stable functionality with a mature implementation


/// @defgroup core-visuals Core functionality related to visuals
/// @ingroup core

/// @defgroup core-audials Core functionality related to audials
/// @ingroup core

/// @defgroup core-hapticals Core functionality related to haptics
/// @ingroup core


/// @defgroup core-file-system Core functionality related to the file system
/// @ingroup core

#include "dx/core/byte_array.h"
#include "dx/core/canvas_msgs.h"
#include "dx/core/convert.h"
#include "dx/core/core.h"
#include "dx/core/count_leading_zeroes.h"
#include "dx/core/file_system.h"
#include "dx/core/hapticals.h"
#include "dx/core/math.h"
#include "dx/core/next_power_of_two.h"
#include "dx/core/os.h"
#include "dx/core/pointer_hashmap.h"
#include "dx/core/safe_add_nx.h"
#include "dx/core/safe_mul_ix.h"
#include "dx/core/safe_mul_nx.h"
#include "dx/core/string.h"
#include "dx/core/pointer_array.h"
#include "dx/core/visuals.h"

#endif // DX_CORE_H_INCLUDED
