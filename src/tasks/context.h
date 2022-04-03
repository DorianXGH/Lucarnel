#pragma once

#include "../includes/memory_structs.h"
#include <stdint.h>
#include <stdbool.h>
#include "regfile.h"

struct context {
    struct regfile regs;
    struct CR3 cr;
    struct PML4E * pml4;
    bool kernel;
};