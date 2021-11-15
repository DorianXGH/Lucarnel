#pragma once 
#include <stdint.h>
#include <stdbool.h>
#include "../includes/stivale.h"
#define BLOCK_SIZE (1 << 20)

struct memory_block_seg {
    uint64_t base;
    uint64_t len; // 0 means the struct is non valid;
    bool     allocated;
};

void register_mmap(struct mmap_entry * base, uint64_t count);
void regroup();
void resort();
void sort();
void * kmalloc(uint64_t size);