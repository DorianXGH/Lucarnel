#pragma once
#include <stdint.h>
#include "../includes/memory_structs.h"

struct PML4E pml4[512] __attribute__ ((aligned (4096)));
struct PDPTE pdpte[512] __attribute__ ((aligned (4096)));

void paging_init_identity();