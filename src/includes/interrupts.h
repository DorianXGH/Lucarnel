#pragma once
#include <stdint.h>
#include "assert.h"

struct IDTD {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) ; 
STATIC_ASSERT(sizeof(struct IDTD) == 10, wrong_IDTD_size);

struct IDTE {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t type_attr; // type and attributes
   uint16_t offset_2; // offset bits 16..31
   uint32_t offset_3; // offset bits 32..63
   uint32_t zero;     // reserved
} __attribute__((packed));
STATIC_ASSERT(sizeof(struct IDTE) == 16, wrong_IDTE_size);