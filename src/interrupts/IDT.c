#include "../includes/interrupts.h"
#include <stdint.h>

struct IDTE IDT[256];

void init_IDT()
{
    IDT[0] = (struct IDTE){ // div by 0
        ((uintptr_t)ISR_general_handler) & 0xFFFF,
        0x08,       // kernel CS
        0,          // no IST
        0b10001110, // present - kernel - not storage - 64 bit interrupt
        ((uintptr_t)ISR_general_handler >> 16) & 0xFFFF,
        ((uintptr_t)ISR_general_handler >> 32) & 0xFFFFFFFF,
        ((uintptr_t)ISR_general_handler >> 16) & 0xFFFF,
    };
    IDT[1] = (struct IDTE){ // debug
        ((uintptr_t)ISR_general_handler) & 0xFFFF,
        0x08,       // kernel CS
        0,          // no IST
        0b10001110, // present - kernel - not storage - 64 bit interrupt
        ((uintptr_t)ISR_general_handler >> 16) & 0xFFFF,
        ((uintptr_t)ISR_general_handler >> 32) & 0xFFFFFFFF,
        ((uintptr_t)ISR_general_handler >> 16) & 0xFFFF,
    };
    IDT[2] = (struct IDTE){ // NMI
        ((uintptr_t)ISR_general_handler) & 0xFFFF,
        0x08,       // kernel CS
        0,          // no IST
        0b10001110, // present - kernel - not storage - 64 bit interrupt
        ((uintptr_t)ISR_general_handler >> 16) & 0xFFFF,
        ((uintptr_t)ISR_general_handler >> 32) & 0xFFFFFFFF,
        ((uintptr_t)ISR_general_handler >> 16) & 0xFFFF,
    };
}