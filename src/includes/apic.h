#pragma once
#include <stdint.h>
#include "assert.h"

struct APICRegister 
{
    uint32_t reg;
    uint32_t reserved[3];
};
STATIC_ASSERT(sizeof(struct APICRegister) == 16, wrong_APIC_register_size);

struct APICConfig
{
    struct APICRegister reserved1[2];
    struct APICRegister LAPIC_ID;
    struct APICRegister LAPIC_version;
    struct APICRegister reserved2[4];
    struct APICRegister task_priority;
    struct APICRegister arbitration_priority;
    struct APICRegister processor_priority;
    struct APICRegister end_of_interrupt;
    struct APICRegister remote_read;
    struct APICRegister logical_destination;
    struct APICRegister destination_format;
    struct APICRegister spurious_interrupt_vector;
    struct APICRegister in_service[8];
    struct APICRegister trigger_mode[8];
    struct APICRegister interrupt_request[8];
};