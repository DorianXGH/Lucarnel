#pragma once
#include <stdint.h>

extern uint8_t* smp_start;
extern uint8_t* smp_end;


void smp_long_bootstrap() __attribute__((section ("long_btstrp")));

void smp_bootstrap_install();