/// @file dx/core/file_system.h
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @brief Core functionality related to the file system.

#if !defined(DX_CORE_FILE_SYSTEM_H_INCLUDED)
#define DX_CORE_FILE_SYSTEM_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core-file-system
/// @brief An enumeration of file access modes to files.
/// @remarks The enumeration elements can be combined.
typedef enum DX_FILE_ACCESS_MODE {

  /// @brief Read access.
  /// @remark Can be combined with DX_FILE_ACCESS_MODE_WRITE.
  DX_FILE_ACCESS_MODE_READ = 1,

  /// @brief Write access.
  /// @remark Can be combined with DX_FILE_ACCESS_MODE_READ.
  DX_FILE_ACCESS_MODE_WRITE = 2,

  /// @brief Read and write access.
  /// @remark Alias for @code{DX_FILE_ACCESS_MODE_READ|DX_FILE_ACCESS_MODE_WRITE}.
  /// Alias for @code{DX_FILE_ACCESS_MODE_WRITE_READ}.
  DX_FILE_ACCESS_MODE_READ_WRITE = DX_FILE_ACCESS_MODE_READ | DX_FILE_ACCESS_MODE_WRITE,

  /// @brief Write and read access.
  /// @remark Alias for @code{DX_FILE_ACCESS_MODE_WRITE|DX_FILE_ACCESS_MODE_READ}.
  /// Alias for @code{DX_FILE_ACCESS_MODE_READ_WRITE}.
  DX_FILE_ACCESS_MODE_WRITE_READ = DX_FILE_ACCESS_MODE_WRITE | DX_FILE_ACCESS_MODE_READ,

} DX_FILE_ACCESS_MODE;

/// @ingroup core-file-system
/// @brief An enumeration of policies for opening a file in case of that the file exists.
/// @remarks The enumeration elements can not be combined.
typedef enum DX_EXISTING_FILE_POLICY {

  /// @brief Retain the file.
  DX_EXISTING_FILE_POLICY_RETAIN,

  /// @brief Truncate the file.
  DX_EXISTING_FILE_POLICY_TRUNCATE,

} DX_EXISTING_FILE_POLICY;

/// @ingroup core-file-system
/// @brief An enumeration of policies for opening a file in case of that the file does not exist.
/// @remarks The enumeration elements can not be combined.
typedef enum DX_NON_EXISTING_FILE_POLICY {

  /// @brief Fail.
  DX_NON_EXISTING_FILE_POLICY_FAIL,

  /// @brief Create the file.
  DX_NON_EXISTING_FILE_POLICY_CREATE,

} DX_NON_EXISTING_FILE_POLICY;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core-file-system
/// @brief
/// Get the contents of a file.
/// @param path
/// The path to the file.
/// @param bytes
/// A pointer to a <code>char*</code> variable.
/// @param number_of_bytes
/// A pointer to a <code>size_t</code> variable.
/// @return
/// The zero value on success. A non-zero value on failure.
/// @success
/// <code>*number_of_bytes</code> was assigned the length, in Bytes, of the contents.
/// <code>*bytes</code> was assigned a pointer to an array of that length.
/// That array must be deallocated using free() when no long required.
/// @failure
/// The error variable was set.
int dx_get_file_contents(char const *path, char **bytes, size_t *number_of_bytes);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core-file-system
/// @brief Enumeration of file states.
typedef enum DX_FILE_STATE {

  /// @brief Unable to determine the file's state.
  DX_FILE_STATE_UNKNOWN,

  /// @brief A file was found and is a director file.
  DX_FILE_STATE_ISDIRECTORY,
  
  /// @brief A file was found and is a regular file.
  DX_FILE_STATE_ISREGULAR,
  
  /// @brief A file was not found.
  DX_FILE_STATE_NOTFOUND,
  
} DX_FILE_STATE;

/// @ingroup core-file-system
/// @brief
/// Get the tate of a fil
/// @param path
/// The path to the file.
/// This function returns a DX_FILE_STATE value not equal to DX_FILE_STATE_UNKNOWN on success.
/// It returns DX_FILE_STATE_UNKNOWN on failure.
/// @failure
/// The error variable was set. 
DX_FILE_STATE dx_get_file_state(char const* path);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_FILE_SYSTEM_H_INCLUDED
