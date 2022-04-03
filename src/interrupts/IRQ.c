#include "../includes/interrupts.h"
#include "../includes/apic.h"
#include "../includes/stivale.h"
#include "../video/video.h"
#include "../includes/x86_64_utils.h"
#include "../includes/utils.h"
#include "../tasks/context.h"

uint64_t tick = 0;
uint64_t stick = 0;
uint8_t tickstr[19];
struct context next_task;

extern struct APICConfig* lapic_regs;


void IRQ_timer(struct regfile * saved_task) { //struct IFrame* interrupt_frame
    tick++;
    if(tick%1000 == 0) {
        putString("tick",0,0,0xFFFFFFFF,0xFFFF0000,1);
        itohex(stick,tickstr);
        putString(tickstr,40,0,0xFFFFFFFF,0xFFFF0000,1);
        stick++;
    }
    outb(0x20, 0x20);
    lapic_regs->end_of_interrupt.reg = 0;
    if(next_task.regs.error && (tick%3000 == 0))
    {
        putString("switch",0,0,0xFF000000,0xFFFFFF00,1);
        next_task.regs.frame.RFLAGS = saved_task->frame.RFLAGS;
        //next_task.regs.frame.CS = saved_task->frame.CS;
        //next_task.regs.frame.SS = saved_task->frame.SS;
        *saved_task = next_task.regs;
        uint64_t * crview = (uint64_t *)(&(next_task.cr));
        itohex(*crview,tickstr);
        putString(tickstr,40,0,0xFF000000,0xFFFFFF00,1);
        //while(1);
        _lcr3(&(next_task.cr));
        //while(1);
    }
}

uint64_t get_current_tick()
{
    return tick;
}