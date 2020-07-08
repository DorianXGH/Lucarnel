#pragma once
#include <stdint.h>

struct GDTD {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) ; 

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

struct CR3 {
    uint16_t PCID: 12;
    uint64_t address: 42;
    uint16_t nullbits: 10;
} __attribute__((packed)) ;

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

struct PDPTE {
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

struct PDE {
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