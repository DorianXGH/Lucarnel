#include "../includes/apic.h"
#include "../includes/stivale.h"
#include "../video/video.h"

extern struct stivale_struct stivale_global_info;
struct APICConfig* lapic_regs;

void init_lapic(struct APICConfig* apicconf)
{
    lapic_regs = apicconf;
    apicconf->spurious_interrupt_vector.reg = 0x0FF;
    __asm__ volatile ("sti");
    putString("pit init wait\0",0,200,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
    init_pit();
    putString("pit init done\0",0,200,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
    
    apicconf->timer_divide_configuration.reg = 0x3; // divide by 16
    apicconf->timer_initial_count.reg = 0xFFF; // count from -1
    apicconf->spurious_interrupt_vector.reg = 0x1FF; // enable apic + set spurious to FF;
    apicconf->LVT_timer.reg = 0x20020;
    uint64_t strt = get_current_tick();
    while(1)//get_current_tick() < strt + 10
    {
        
    }
    
}