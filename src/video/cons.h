#pragma once
#include <stdint.h>
#include "video.h"
#include "../includes/utils.h"
#define DEFAULT_TERM_WIDTH 48
#define DEFAULT_TERM_HEIGHT 32

struct terminal_handle {
    uint64_t x;
    uint64_t y;

    uint64_t w;
    uint64_t h;

    uint32_t background;
    uint32_t foreground;

    uint8_t zoom;

    uint8_t * buffer;
    uint64_t i;
    uint64_t j;
};

void print(uint8_t * str);
void print_num(uint64_t num);
void switch_term(struct terminal_handle * handle);