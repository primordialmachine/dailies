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
#include "dx/core/memory.h"
#include "dx/core/msgs.h"
#include "dx/core/next_power_of_two.h"
#include "dx/core/object.h"
#include "dx/core/object_array.h"
#include "dx/core/os.h"
#include "dx/core/pointer_hashmap.h"
#include "dx/core/safe_add_nx.h"
#include "dx/core/safe_mul_ix.h"
#include "dx/core/safe_mul_nx.h"
#include "dx/core/string.h"
#include "dx/core/pointer_array.h"
#include "dx/core/visuals.h"


/// @brief The name of this library.
#define DX_CORE_NAME "Primordial Machine's Core"

/// @brief The major version of this library.
#define DX_CORE_MAJOR_VERSION (0)

/// @brief The minor version of this library.
#define DX_CORE_MINOR_VERSION (0)

/// @brief The revision version of this library.
#define DX_CORE_REVISION_VERSION (1)


/// @brief Macro evaluating to <code>__FILE__</code>.
#define DX_C_FILE_NAME __FILE__

/// @brief Macro evaluating to <code>__LINE__</code>.
#define DX_C_FILE_LINE __LINE__

/// @brief Macro evaluating to <code>__func__</code>.
#define DX_C_FUNCTION_NAME __func__

#endif // DX_CORE_H_INCLUDED
