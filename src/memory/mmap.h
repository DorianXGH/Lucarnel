#pragma once 
#include <stdint.h>
#include <stdbool.h>
#include "../includes/stivale.h"
#include "../video/cons.h"
#define MAX_ALLOC (1 << 14) // 16K allocations

struct memory_block_seg {
    uint64_t base;
    uint64_t len; // 0 means the struct is non valid;
    uint8_t  allocated :1;
    uint8_t  bitmap_allocated :1;
    uint8_t  full :1;
    uint64_t allocated_count :59;
    uint64_t first_free;
} __attribute__((packed));


void register_mmap(struct mmap_entry * base, uint64_t count);

void regroup();

bool compare_len(struct memory_block_seg * i, struct memory_block_seg * j);
bool compare_base(struct memory_block_seg * i, struct memory_block_seg * j);

void sort_insert(uint64_t l, uint64_t r, uint64_t sz, bool(* comp)(struct memory_block_seg *,struct memory_block_seg *));
void sort();
void * pmalloc_constraint(uint64_t size, uint64_t max_add);
void * pmalloc(uint64_t size);
void free(void * ptr);

void show_map();