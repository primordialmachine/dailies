#include "dx/core/file_system.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/byte_array.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_get_file_contents(char const *path, char **bytes, size_t *number_of_bytes) {
  dx_byte_array byte_array;
  if (dx_byte_array_construct(&byte_array)) {
    return 1;
  }
  HANDLE file = CreateFile(path,
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           0,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           0);
  if (INVALID_HANDLE_VALUE == file) {
    dx_byte_array_destruct(&byte_array);
    return 1;
  }
  while (true) {
    char temporary[1024]; DWORD requested = 1024;
    DWORD received = 0;
    BOOL result = ReadFile(file, temporary, requested, &received, NULL);
    if (!result) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      dx_byte_array_destruct(&byte_array);
      return 1;
    }
    // eof
    if (received == 0) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      break;
    }
    if (dx_byte_array_append(&byte_array, temporary, received)) {
      CloseHandle(file);
      file = INVALID_HANDLE_VALUE;
      dx_byte_array_destruct(&byte_array);
      return 1;
    }
  }
  
  char *bytes_1; size_t number_of_bytes_1;
  if (dx_byte_array_steal(&byte_array, &bytes_1, &number_of_bytes_1)) {
    dx_byte_array_destruct(&byte_array);
    return 1;
  }
  dx_byte_array_destruct(&byte_array);
  *bytes = bytes_1;
  *number_of_bytes = number_of_bytes_1;
  
  return 0;
}

DX_FILE_STATE dx_get_file_state(char const* path) {
  DWORD t = GetFileAttributesA(path);
  if (t == INVALID_FILE_ATTRIBUTES) {
    t = GetLastError();
    SetLastError(0);
    if (t == ERROR_FILE_NOT_FOUND ||
        t == ERROR_PATH_NOT_FOUND ||
        t == ERROR_INVALID_NAME ||
        t == ERROR_INVALID_DRIVE ||
        t == ERROR_NOT_READY ||
        t == ERROR_INVALID_PARAMETER ||
        t == ERROR_BAD_PATHNAME ||
        t == ERROR_BAD_NETPATH)
    {
      return DX_FILE_STATE_NOTFOUND;
    } else {
      dx_set_error(DX_ENVIRONMENT_FAILED);
      return DX_FILE_STATE_UNKNOWN;
    }
  } else {
    if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & t))
    {
      return DX_FILE_STATE_ISDIRECTORY;
    }
    else
    {
      return DX_FILE_STATE_ISREGULAR;
    }
  }
}
