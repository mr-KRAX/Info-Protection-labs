#pragma once

#include <cstdlib>
#include <cstdint>


void s64to32(uint64_t block64b, uint32_t* block32b_1, uint32_t* block32b_2);

void s64to8(uint64_t block64b, uint8_t* blocks8b);

void s32to8(uint32_t block32b, uint8_t* blocks8b);

uint64_t j32to64(uint32_t block32b_1, uint32_t block32b_2);

uint64_t j8to64(uint8_t* blocks8b);

uint64_t j8to32(uint8_t* blocks8b);

void swap(uint32_t* N1, uint32_t* N2);