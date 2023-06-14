#if !defined(DX_CORE_OS_H_INCLUDED)
#define DX_CORE_OS_H_INCLUDED

#include "dx/core/string.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup os
/// @brief Get the page size.
/// @return The page size on success. @a 0 on failure.
/// @failure This function has set the error variable.
size_t dx_os_get_page_size();

/// @ingroup os
/// @brief Get the number of CPU cores.
/// @return The number of cores on success. @a 0 on failure.
/// @failure This function has set the error variable.
size_t dx_os_get_number_of_cores();

/// @ingroup os
/// @brief Get the executable path.
/// @return A pointer to the executable path dx_string object. The null pointer on failure.
/// @success The caller acquired a reference to the string.
dx_string* dx_os_get_executable_path();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_OS_H_INCLUDED
