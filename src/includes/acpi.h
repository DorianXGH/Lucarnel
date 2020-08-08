#pragma once
#include <stdint.h>
#include <stdbool.h>

struct RSDP
{
    uint8_t signature[8];
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

struct RSDP2
{
    struct RSDP header;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct ACPISDTHeader
{
    uint8_t signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t OEMtableID[8];
    uint32_t OEMrevision;
    uint32_t creatorID;
    uint32_t creator_revision;
} __attribute__((packed));

bool do_checksum(struct ACPISDTHeader *table_header);