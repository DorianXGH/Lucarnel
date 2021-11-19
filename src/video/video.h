#pragma once
#include <stdint.h>
#include "../includes/stivale.h"

void initCharMap();
void putChar(uint8_t c, uint64_t x, uint64_t y, uint32_t foreground, uint32_t background, uint8_t zoomfactor);
void putString(uint8_t* s, uint64_t x, uint64_t y, uint32_t foreground, uint32_t background, uint8_t zoomfactor);
void register_framebuffer(struct stivale_struct *stivale);
void clear();