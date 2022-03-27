#pragma once

#include "../includes/memory_structs.h"
#include "../includes/interrupts.h"
#include <stdint.h>
#include <stdbool.h>

struct regfile {
    uint64_t r15,r14,r13,r12,r11,r10,r9,r8,rbp,rsi,rdi,rdx,rcx,rbx,rax;
    uint64_t error;
    struct IFrame frame;
};