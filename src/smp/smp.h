#pragma once
#include <stdint.h>
#include "../video/video.h"
#include "../includes/stivale.h"
#include "../includes/x86_64_utils.h"
#include "../includes/interrupts.h"
#include "../includes/memory_structs.h"

extern uint8_t* smp_start;
extern uint8_t* smp_end;
extern struct IDTD idtd;
extern struct GDTD gdtd;
extern struct PML4E pml4[512];
extern int init_system();

void smp_long_bootstrap() __attribute__((section ("long_btstrp")));

void smp_bootstrap_install();