#include "dx/core/os.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "dx/core/memory.h"
#include "dx/core/safe_add_nx.h"

// SYSTEM_INFO, GetSystemInfo
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_size dx_os_get_page_size() {
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  if (system_info.dwPageSize > SIZE_MAX) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 0;
  }
  return (dx_size)system_info.dwPageSize;
}

dx_size dx_os_get_number_of_cores() {
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  if (system_info.dwNumberOfProcessors > SIZE_MAX) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 0;
  }
  return (dx_size)system_info.dwNumberOfProcessors;
}

dx_string* dx_os_get_executable_path() {
  HMODULE module = GetModuleHandleA(NULL);
  if (!module) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return NULL;
  }
  dx_size n = MAX_PATH;
  char *p = dx_memory_allocate(n);
  if (!p) {
    return NULL;
  }
  do {
    DWORD m = GetModuleFileNameA(module, p, n);
    if (m == 0) {
      dx_memory_deallocate(p);
      p = NULL;
      dx_set_error(DX_ENVIRONMENT_FAILED);
      return NULL;
    }
    // If m is non-zero, then the function succeeded.
    // There are now two cases:
    // a) m == n: The buffer was too small (n is non-zero).
    // b) m < n: m denotes the number of characters copied without the zero terminator.
    if (m == n) {
      dx_size overflow;
      dx_size n_new = dx_add_sz(n, MAX_PATH, &overflow);
      if (overflow) {
        n_new = SIZE_MAX;
      }
      if (n_new == n) {
        dx_memory_deallocate(p);
        p = NULL;
        dx_set_error(DX_ALLOCATION_FAILED);
        return NULL;
      }
      char* p_new = dx_memory_reallocate(p, n_new);
      if (!p_new) {
        dx_memory_deallocate(p);
        p = NULL;
        dx_set_error(DX_ALLOCATION_FAILED);
        return NULL;
      }
      p = p_new;
      n = n_new;
    } else {
      dx_string* s = dx_string_create(p, m);
      dx_memory_deallocate(p);
      return s;
    }
  } while (true);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
