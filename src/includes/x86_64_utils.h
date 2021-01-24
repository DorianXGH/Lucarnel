#pragma once
#include "memory_structs.h"
#include "interrupts.h"

void _lgdt(struct GDTD *);
void _lidt(struct IDTD *);
void _lcr3(struct CR3 *);

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile( "outb %0, %1" : : "Nd"(port), "a"(val) );
}
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %0, %1"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline void io_wait()
{
    outb(0x80, 0);
}