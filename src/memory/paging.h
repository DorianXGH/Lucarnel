#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../includes/memory_structs.h"
#include "../includes/x86_64_utils.h"

void paging_init_identity();
void map(uint64_t vbase, uint64_t pbase, uint64_t base, bool cache);
