#pragma once
#include "memory_structs.h"

void _lgdt(struct GDTD *);
void _lidt(struct IDTD *);
void _lcr3(struct CR3 *);

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}