#include "../includes/apic.h"
#include "../includes/stivale.h"
#include "../video/video.h"

extern struct stivale_struct stivale_global_info;

void init_lapic(struct APICConfig* apicconf)
{
    putString("pit init wait\0",0,200,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
    init_pit();
    putString("pit init done\0",0,200,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
    apicconf->spurious_interrupt_vector.reg = 0x1FF; // enable apic + set spurious to FF;
    apicconf->timer_divide_configuration.reg = 0x3; // divide by 16
    apicconf->timer_initial_count.reg = 0xFFFFFFFF; // count from -1
    uint64_t strt = get_current_tick();
    while(get_current_tick() < strt + 10)
    {

    }

}