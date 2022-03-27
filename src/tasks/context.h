#pragma once

#include "../includes/memory_structs.h"
#include <stdint.h>
#include <stdbool.h>
#include "regfile.h"

struct context {
    struct regfile regs;
    uint64_t rip, rflags;
    uint16_t cs,ds,ss,es,fs,gs;
    struct CR3 cr;
    struct PML4E * pml4;
    bool kernel;
};