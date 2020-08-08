#pragma once
#include <stdint.h>
#include <stdbool.h>

void itohex(uint64_t num, uint8_t buffer[19]);
bool arrcmp(uint8_t* arr1, uint8_t* arr2, uint8_t len);