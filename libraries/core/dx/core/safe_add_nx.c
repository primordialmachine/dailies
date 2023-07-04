#include "dx/core/safe_add_nx.h"

uint8_t dx_add_u8(uint8_t a, uint8_t b, uint8_t* c) {
  uint8_t t = a + b;
  *c = UINT8_MAX - a < b;
  return t;
}

uint16_t dx_add_u16(uint16_t a, uint16_t b, uint16_t* c) {
  uint16_t t = a + b;
  *c = UINT16_MAX - a < b;
  return t;
}

uint32_t dx_add_u32(uint32_t a, uint32_t b, uint32_t* c) {
  uint32_t t = a + b;
  *c = UINT32_MAX - a < b;
  return t;
}

uint64_t dx_add_u64(uint64_t a, uint64_t b, uint64_t* c) {
  uint64_t t = a + b;
  *c = UINT64_MAX - a < b;
  return t;
}

dx_size dx_add_sz(dx_size a, dx_size b, dx_size* c) {
  dx_size t = a + b;
  *c = SIZE_MAX - a < b;
  return t;
}

#if defined(DX_SAFE_ADD_NX_WITH_TESTS) && 1 == DX_SAFE_ADD_NX_WITH_TESTS

static int dx_safe_add_n8_tests() {
  dx_n8 u, v;

  // 0 + 0 = (0, 0)
  u = dx_add_u8(0, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // MAX + 0 = (MAX, 0)
  u = dx_add_u8(DX_N8_GREATEST, 0, &v);
  if (u != DX_N8_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // 0 + MAX = (MAX, 0)
  u = dx_add_u8(0, DX_N8_GREATEST, &v);
  if (u != DX_N8_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // MAX + 1 = (0, 1)
  u = dx_add_u8(DX_N8_GREATEST, 1, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 1 + MAX = (0, 1)
  u = dx_add_u8(1, DX_N8_GREATEST, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + 2 = (1, 1)
  u = dx_add_u8(DX_N8_GREATEST, 2, &v);
  if (u != 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 2 + MAX = (1, 1)
  u = dx_add_u8(2, DX_N8_GREATEST, &v);
  if (u != 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + 37 = (36, 1)
  u = dx_add_u8(DX_N8_GREATEST, 37, &v);
  if (u != 36) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 37 + MAX = (36, 1)
  u = dx_add_u8(37, DX_N8_GREATEST, &v);
  if (u != 36) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + MAX = (MAX - 1,1)
  u = dx_add_u8(DX_N8_GREATEST, DX_N8_GREATEST, &v);
  if (u != DX_N8_GREATEST - 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX+MAX=(MAX-1,1)
  u = dx_add_u8(DX_N8_GREATEST, DX_N8_GREATEST, &v);
  if (u != DX_N8_GREATEST - 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }

  return 0;
}

static int dx_safe_add_n16_tests() {
  dx_n16 u, v;

  // 0 + 0 = (0, 0)
  u = dx_add_u16(0, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // MAX + 0 = (MAX, 0)
  u = dx_add_u16(DX_N16_GREATEST, 0, &v);
  if (u != DX_N16_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // 0 + MAX = (MAX, 0)
  u = dx_add_u16(0, DX_N16_GREATEST, &v);
  if (u != DX_N16_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // MAX + 1 = (0, 1)
  u = dx_add_u16(DX_N16_GREATEST, 1, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 1 + MAX = (0, 1)
  u = dx_add_u16(1, DX_N16_GREATEST, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + 2 = (1, 1)
  u = dx_add_u16(DX_N16_GREATEST, 2, &v);
  if (u != 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 2 + MAX = (1, 1)
  u = dx_add_u16(2, DX_N16_GREATEST, &v);
  if (u != 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + 37 = (36, 1)
  u = dx_add_u16(DX_N16_GREATEST, 37, &v);
  if (u != 36) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 37 + MAX = (36, 1)
  u = dx_add_u16(37, DX_N16_GREATEST, &v);
  if (u != 36) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + MAX = (MAX - 1, 1)
  u = dx_add_u16(DX_N16_GREATEST, DX_N16_GREATEST, &v);
  if (u != DX_N16_GREATEST - 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + MAX = (MAX - 1, 1)
  u = dx_add_u16(DX_N16_GREATEST, DX_N16_GREATEST, &v);
  if (u != DX_N16_GREATEST - 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }

  return 0;
}

static int dx_safe_add_n32_tests() {
  dx_n32 u, v;

  // 0 + 0 = (0, 0)
  u = dx_add_u32(0, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // MAX + 0 = (MAX, 0)
  u = dx_add_u32(DX_N32_GREATEST, 0, &v);
  if (u != DX_N32_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // 0 + MAX = (MAX, 0)
  u = dx_add_u32(0, DX_N32_GREATEST, &v);
  if (u != DX_N32_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // MAX + 1 = (0, 1)
  u = dx_add_u32(DX_N32_GREATEST, 1, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 1 + MAX = (0, 1)
  u = dx_add_u32(1, DX_N32_GREATEST, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + 2 = (1, 1)
  u = dx_add_u32(DX_N32_GREATEST, 2, &v);
  if (u != 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 2 + MAX = (1, 1)
  u = dx_add_u32(2, DX_N32_GREATEST, &v);
  if (u != 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + 37 = (36, 1)
  u = dx_add_u32(DX_N32_GREATEST, 37, &v);
  if (u != 36) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 37 + MAX = (36, 1)
  u = dx_add_u32(37, DX_N32_GREATEST, &v);
  if (u != 36) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + MAX = (MAX - 1, 1)
  u = dx_add_u32(DX_N32_GREATEST, DX_N32_GREATEST, &v);
  if (u != DX_N32_GREATEST - 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + MAX = (MAX - 1, 1)
  u = dx_add_u32(DX_N32_GREATEST, DX_N32_GREATEST, &v);
  if (u != DX_N32_GREATEST - 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }

  return 0;
}

static int dx_safe_add_n64_tests() {
  dx_n64 u, v;

  // 0 + 0 = (0, 0)
  u = dx_add_u64(0, 0, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // MAX + 0 = (MAX, 0)
  u = dx_add_u64(DX_N64_GREATEST, 0, &v);
  if (u != DX_N64_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // 0 + MAX = (MAX, 0)
  u = dx_add_u64(0, DX_N64_GREATEST, &v);
  if (u != DX_N64_GREATEST) {
    return 1;
  }
  if (v != 0) {
    return 1;
  }
  // MAX + 1 = (0, 1)
  u = dx_add_u64(DX_N64_GREATEST, 1, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 1 + MAX = (0, 1)
  u = dx_add_u64(1, DX_N64_GREATEST, &v);
  if (u != 0) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + 2 = (1, 1)
  u = dx_add_u64(DX_N64_GREATEST, 2, &v);
  if (u != 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 2 + MAX = (1, 1)
  u = dx_add_u64(2, DX_N64_GREATEST, &v);
  if (u != 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + 37 = (36, 1)
  u = dx_add_u64(DX_N64_GREATEST, 37, &v);
  if (u != 36) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // 37 + MAX = (36, 1)
  u = dx_add_u64(37, DX_N64_GREATEST, &v);
  if (u != 36) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + MAX = (MAX - 1, 1)
  u = dx_add_u64(DX_N64_GREATEST, DX_N64_GREATEST, &v);
  if (u != DX_N64_GREATEST - 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }
  // MAX + MAX = (MAX - 1, 1)
  u = dx_add_u64(DX_N64_GREATEST, DX_N64_GREATEST, &v);
  if (u != DX_N64_GREATEST - 1) {
    return 1;
  }
  if (v != 1) {
    return 1;
  }

  return 0;
}

int dx_safe_add_nx_tests() {
  if (dx_safe_add_n8_tests()) {
    return 1;
  }
  if (dx_safe_add_n16_tests()) {
    return 1;
  }
  if (dx_safe_add_n32_tests()) {
    return 1;
  }
  if (dx_safe_add_n64_tests()) {
    return 1;
  }
  return 0;
}

#endif // DX_SAFE_ADD_NX_WITH_TESTS
