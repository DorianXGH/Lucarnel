#include "../includes/interrupts.h"
#include "../includes/x86_64_utils.h"
#include <stdint.h>
#define IDT_SIZE 3

struct IDTE IDT[256];
struct IDTD idtd;

struct IDTE gen_IDT_entry(uintptr_t handler)
{
    return (struct IDTE){ // div by 0
        handler & 0xFFFF,
        0x08,       // kernel CS
        0,          // no IST
        0b10001110, // present - kernel - not storage - 64 bit interrupt
        (handler >> 16) & 0xFFFF,
        (handler >> 32) & 0xFFFFFFFF,
        0,
    };
}

void init_IDT()
{
    IDT[0] = gen_IDT_entry((uintptr_t)ISR_div_by_zero_handler); // div by 0
    IDT[1] = gen_IDT_entry((uintptr_t)ISR_debug_handler); // debug
    IDT[2] = gen_IDT_entry((uintptr_t)ISR_NMI_handler); // NMI
    idtd = (struct IDTD) {
        (IDT_SIZE*16)-1,
        &IDT
    };
    _lidt(&idtd);
}