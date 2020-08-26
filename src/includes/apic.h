#pragma once
#include <stdint.h>
#include "assert.h"
#include "interrupts.h"
#include "x86_64_utils.h"

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
    struct APICRegister error_status;
    struct APICRegister reserved3[6];
    struct APICRegister LVT_corrected_machine_check_interrupt;
    struct APICRegister interrupt_command[2];
    struct APICRegister LVT_timer;
    struct APICRegister LVT_thermal_sensor;
    struct APICRegister LVT_performance_monitoring_counters;
    struct APICRegister LVT_LINT0;
    struct APICRegister LVT_LINT1;
    struct APICRegister LVT_error;
    struct APICRegister timer_initial_count;
    struct APICRegister timer_current_count;
    struct APICRegister reserved4[4];
    struct APICRegister timer_divide_configuration;
    struct APICRegister reserved5;

};
STATIC_ASSERT(sizeof(struct APICConfig) == 0x400, wrong_APIC_config_size);


void init_lapic(struct APICConfig* apicconf);
/*
void send_INIT_IPI(struct APICConfig* apicconf);
void send_STARTUP_IPI(struct APICConfig* apicconf,uint8_t pagenum);
*/