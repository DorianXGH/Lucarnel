#include "../includes/interrupts.h"
#include "../includes/apic.h"
#include "../includes/stivale.h"
#include "../video/video.h"
#include "../includes/x86_64_utils.h"
#include "../includes/utils.h"

uint64_t tick = 0;

extern struct stivale_struct stivale_global_info;
extern struct APICConfig* lapic_regs;

__attribute__((interrupt)) void IRQ_timer(struct IFrame* interrupt_frame) {
    tick++;
    putString("tick",0,0,&stivale_global_info,0xFFFFFFFF,0xFFFF0000,1);
    uint8_t tickstr[19];
    itohex(tick,tickstr);
    putString(tickstr,40,0,&stivale_global_info,0xFFFFFFFF,0xFFFF0000,1);
    outb(0x20, 0x20);
    lapic_regs->end_of_interrupt.reg = 0;
}

uint64_t get_current_tick()
{
    return tick;
}