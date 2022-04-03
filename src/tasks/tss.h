#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../includes/memory_structs.h"

struct TSS {
    uint32_t reserved;
    uint64_t rsp[3];
    uint64_t reserved2;
    uint64_t ist[7];
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t iopb;
} __attribute__((packed));

void install_TSS();