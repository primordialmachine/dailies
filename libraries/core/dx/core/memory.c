#include "dx/core/memory.h"

// malloc, realloc, free
#include <malloc.h>

// memmove, memcpy, memset
#include <string.h>

int dx_memory_copy(void* p, void const* q, dx_size n) {
  if (!p || !q) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  memcpy(p, q, n);
  return 0;
}

int dx_memory_move(void* p, void const* q, dx_size n) {
  if (!p || !q) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  memmove(p, q, n);
  return 0;
}

int dx_memory_zero(void* p, dx_size n) {
  if (!p) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  memset(p, 0, n);
  return 0;
}

void* dx_memory_allocate(dx_size n) {
  void* p = malloc(n > 0 ? n : 1);
  if (!p) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return NULL;
  }
  return p;
}

void* dx_memory_reallocate(void* p, dx_size n) {
  if (!p) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return NULL;
  }
  void *q = realloc(p, n > 0 ? n : 1);
  if (!q) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return  NULL;
  }
  return q;
}

void dx_memory_deallocate(void* p) {
  if (p) {
    free(p);
  }
}

int dx_memory_compare(void const* p, void const* q, dx_size n) {
  if (!p || !q) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 0;
  }
  return !memcmp(p, q, n);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int dx_allocator_statistics_initialize(dx_allocator_statistics *statistics) {
	if (!statistics) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
	}
	DX_DEBUG_ASSERT(NULL != statistics);
	statistics->number_of_blocks = 0;
	statistics->smallest_block = 0;
	statistics->greatest_block = 0;
	statistics->number_of_bytes = 0;
	statistics->number_of_kilo_bytes = 0;
	return 0;
}

void dx_allocator_statistics_uninitialize(dx_allocator_statistics *statistics) {
	DX_DEBUG_ASSERT(NULL != statistics);
	statistics->number_of_blocks = 0;
	statistics->smallest_block = 0;
	statistics->greatest_block = 0;
	statistics->number_of_bytes = 0;
	statistics->number_of_kilo_bytes = 0;
}

int dx_allocator_statistics_on_block_allocated(dx_allocator_statistics* statistics, dx_size number_of_bytes) {
	if (!statistics) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
	}
	dx_size number_of_kilo_bytes = 0;
	while (number_of_bytes > 1000) {
		number_of_bytes -= 1000;
		number_of_kilo_bytes += 1;
	}
  if (DX_SIZE_GREATEST - statistics->number_of_bytes < number_of_bytes) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;
  }
  if (DX_SIZE_GREATEST - statistics->number_of_kilo_bytes < number_of_kilo_bytes) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;   
  }
  if (DX_SIZE_GREATEST - statistics->number_of_blocks < 1) {
    dx_set_error(DX_ALLOCATION_FAILED);
    return 1;   
  }
	statistics->number_of_bytes += number_of_bytes;
	statistics->number_of_kilo_bytes += number_of_kilo_bytes;
	statistics->number_of_blocks += 1;
	if (statistics->smallest_block > number_of_bytes) {
		statistics->smallest_block = number_of_bytes;
	}
	if (statistics->greatest_block < number_of_bytes) {
		statistics->greatest_block = number_of_bytes;
	}
	return 0;
}

int dx_allocator_statistics_on_block_deallocated(dx_allocator_statistics* statistics, dx_size number_of_bytes) {
	if (!statistics) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
	}
  dx_size number_of_kilo_bytes = 0;
	while (number_of_bytes > 1000) {
		number_of_bytes -= 1000;
		number_of_kilo_bytes += 1;
	}
	statistics->number_of_blocks -= 1;
	statistics->number_of_kilo_bytes -= number_of_kilo_bytes;
	statistics->number_of_bytes -= number_of_bytes;
	return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
