#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "acpi.h"
#include "utils.h"

struct pci_config_header_common {
    uint16_t vendor_ID;
    uint16_t device_ID;

    uint16_t command;
    uint16_t status;

    uint8_t rev_ID;
    uint8_t prog_IF;
    uint8_t subclass;
    uint8_t class;

    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
} __attribute__((packed));

struct pci_config_header_device {
    struct pci_config_header_common common;

    uint32_t BAR[6];

    uint32_t cardbus_CIS_ptr;

    uint16_t subsys_vendor_ID;
    uint16_t subsys_ID;

    uint32_t exp_ROM_base;

    uint8_t capabilities_offset;
    uint8_t res1[3];

    uint32_t res2;

    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;
} __attribute__((packed));


void init_pci_tree(struct MCFG* mcfg);

struct pci_config_header_common * get_PCI_device_cfg_space(uint64_t PCI_config_base, uint8_t bus, uint8_t device, uint8_t function);