#include "../includes/interrupts.h"
#include "../includes/apic.h"
#include "../includes/stivale.h"
#include "../video/video.h"
#include "../includes/x86_64_utils.h"
#include "../includes/utils.h"

uint64_t tick = 0;
uint64_t stick = 0;
uint8_t tickstr[19];

extern struct APICConfig* lapic_regs;

__attribute__((naked)) void IRQ_timer() { //struct IFrame* interrupt_frame
    tick++;
    if(tick%1000 == 0) {
        putString("tick",0,0,0xFFFFFFFF,0xFFFF0000,1);
        itohex(stick,tickstr);
        putString(tickstr,40,0,0xFFFFFFFF,0xFFFF0000,1);
        stick++;
    }
    outb(0x20, 0x20);
    lapic_regs->end_of_interrupt.reg = 0;
    __asm__ volatile ("iretq");
}

uint64_t get_current_tick()
{
    return tick;
}