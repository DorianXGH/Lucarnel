#pragma once

#include "../includes/memory_structs.h"
#include <stdint.h>
#include <stdbool.h>

struct context {
    uint64_t rax,rbx,rcx,rdx,rsi,rdi,rsp,rbp,r8,r9,r10,r11,r12,r13,r14,r15;
    uint64_t rip, rflags;
    uint16_t cs,ds,ss,es,fs,gs;
    struct CR3 cr;
    struct PML4E * pml4;
    bool kernel;
};