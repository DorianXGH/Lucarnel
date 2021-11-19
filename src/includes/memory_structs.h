#pragma once
#include <stdint.h>
#include "assert.h"

struct GDTD {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) ; 
STATIC_ASSERT(sizeof(struct GDTD) == 10, wrong_GDTD_size);

struct GDTE {
    uint16_t limit1;
    uint16_t base1;
    uint8_t base2;
    uint8_t accessed: 1;
    uint8_t read_write: 1;
    uint8_t dir_conforming: 1;
    uint8_t code: 1;
    uint8_t descriptor: 1;
    uint8_t ring: 2;
    uint8_t present: 1;
    uint8_t limit2: 4;
    uint8_t nullbits: 1;
    uint8_t longmode: 1;
    uint8_t size: 1;
    uint8_t granularity: 1;
    uint8_t base3: 8;
} __attribute__((packed)) ;
STATIC_ASSERT(sizeof(struct GDTE) == 8, wrong_GDTE_size);


struct GDTE_TSS {
    uint16_t limit1;
    uint16_t base1;
    uint8_t base2;
    uint8_t accessed: 1;
    uint8_t read_write: 1;
    uint8_t dir_conforming: 1;
    uint8_t code: 1;
    uint8_t descriptor: 1;
    uint8_t ring: 2;
    uint8_t present: 1;
    uint8_t limit2: 4;
    uint8_t nullbits: 3;
    uint8_t granularity: 1;
    uint8_t base3: 8;
    uint32_t base4;
    uint32_t reserved;
} __attribute__((packed)) ;
STATIC_ASSERT(sizeof(struct GDTE_TSS) == 16, wrong_GDTE_TSS_size);

struct CR3 {
    uint16_t PCID: 12;
    uint64_t address: 42;
    uint16_t nullbits: 10;
} __attribute__((packed)) ;
STATIC_ASSERT(sizeof(struct CR3) == 8, wrong_CR3_size);

struct PML4E {
    uint8_t present: 1;
    uint8_t RW: 1;
    uint8_t supervisor: 1;
    uint8_t write_through: 1;
    uint8_t cache_disabled: 1;
    uint8_t accessed: 1;
    uint8_t ignored: 1;
    uint8_t nullbits_flg: 5;
    uint64_t address: 42;
    uint16_t nullbits: 9;
    uint8_t execution_disabled: 1;
} __attribute__((packed)) ;
STATIC_ASSERT(sizeof(struct PML4E) == 8, wrong_PML4E_size);

struct PDPTE {
    uint8_t present: 1;
    uint8_t RW: 1;
    uint8_t supervisor: 1;
    uint8_t write_through: 1;
    uint8_t cache_disabled: 1;
    uint8_t accessed: 1;
    uint8_t dirty: 1; // only if following bit is 1
    uint8_t page1G: 1;
    uint8_t nullbits_flg: 4;
    uint64_t address: 42;
    uint16_t nullbits: 9;
    uint8_t execution_disabled: 1;
} __attribute__((packed)) ;
STATIC_ASSERT(sizeof(struct PDPTE) == 8, wrong_PDPTE_size);

struct PDE {
    uint8_t present: 1;
    uint8_t RW: 1;
    uint8_t supervisor: 1;
    uint8_t write_through: 1;
    uint8_t cache_disabled: 1;
    uint8_t accessed: 1;
    uint8_t dirty: 1; // only if following bit is 1
    uint8_t page2M: 1;
    uint8_t nullbits_flg: 4;
    uint64_t address: 42;
    uint16_t nullbits: 9;
    uint8_t execution_disabled: 1;
} __attribute__((packed)) ;
STATIC_ASSERT(sizeof(struct PDE) == 8, wrong_PDE_size);

struct PTE {
    uint8_t present: 1;
    uint8_t RW: 1;
    uint8_t supervisor: 1;
    uint8_t write_through: 1;
    uint8_t cache_disabled: 1;
    uint8_t accessed: 1;
    uint8_t dirty: 1;
    uint8_t global: 1;
    uint8_t nullbits_flg: 4;
    uint64_t address: 42;
    uint16_t nullbits: 9;
    uint8_t execution_disabled: 1;
} __attribute__((packed)) ;
STATIC_ASSERT(sizeof(struct PTE) == 8, wrong_PTE_size);

enum PAGE_SIZE {
    PS_4K,
    PS_2M,
    PS_1G,
};