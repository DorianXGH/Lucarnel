#pragma once

#include "../includes/memory_structs.h"
#include "../includes/interrupts.h"
#include <stdint.h>
#include <stdbool.h>

struct IFrame {
    uint64_t RIP;
    uint64_t CS;
    uint64_t RFLAGS;
    uint64_t RSP;
    uint64_t SS;
} __attribute__((packed));
STATIC_ASSERT(sizeof(struct IFrame) == 40, wrong_interrupt_frame_size);

struct regfile {
    uint64_t r15,r14,r13,r12,r11,r10,r9,r8,rbp,rsi,rdi,rdx,rcx,rbx,rax;
    uint64_t error;
    struct IFrame frame;
} __attribute__((packed));