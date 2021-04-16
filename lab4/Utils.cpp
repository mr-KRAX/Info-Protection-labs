#include <cstdlib>
#include <cstdint>
using namespace std;

void s64to32(uint64_t block64b, uint32_t* block32b_1, uint32_t* block32b_2) {
  *block32b_1 = (uint32_t)(block64b >> 32);
  *block32b_2 = (uint32_t)(block64b);
}

void s64to8(uint64_t block64b, uint8_t* blocks8b) {
  for (size_t i = 0; i < 8; ++i) {
    blocks8b[i] = (uint8_t)(block64b >> ((7 - i) * 8));
  }
}

void s32to8(uint32_t block32b, uint8_t* blocks8b) {
  for (uint8_t i = 0; i < 4; ++i) {
    blocks8b[i] = (uint8_t)(block32b >> (24 - (i * 8)));
  }
}

uint64_t j32to64(uint32_t block32b_1, uint32_t block32b_2) {
  uint64_t block64b;
  block64b = (uint64_t)block32b_1;
  block64b = (uint64_t)(block64b << 32) | block32b_2;
  return block64b;
}

uint64_t j8to64(uint8_t* blocks8b) {
  uint64_t block64b;
  for (uint8_t* p = blocks8b; p < blocks8b + 8; ++p) {
    block64b = (block64b << 8) | *p;
  }
  return block64b;
}

uint64_t j8to32(uint8_t* blocks8b) {
  uint64_t block32b;
  for (uint8_t* p = blocks8b; p < blocks8b + 4; ++p) {
    block32b = (block32b << 8) | *p;
  }
  return block32b;
}

void swap(uint32_t* N1, uint32_t* N2) {
  uint32_t temp = *N1;
  *N1 = *N2;
  *N2 = temp;
}