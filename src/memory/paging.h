#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../includes/memory_structs.h"
#include "../includes/x86_64_utils.h"
#include "../tasks/context.h"
#include "mmap.h"

void paging_init_identity();
void map(struct context * ctx, uint64_t vbase, uint64_t pbase, uint64_t size, bool cache_dis, bool code, bool supervisor);
void map_page(struct context * ctx, uint64_t vp, uint64_t pp, bool cache_dis, bool code, bool supervisor, enum PAGE_SIZE sz);
