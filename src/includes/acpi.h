#pragma once
#include <stdint.h>

struct RSDP {
    uint8_t signature[8];
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

struct RSDP2 {
    RSDP header;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));
