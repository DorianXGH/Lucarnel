#include "../includes/apic.h"

void init_lapic(struct APICConfig* apicconf)
{
    init_pit();
    apicconf->spurious_interrupt_vector.reg = 0x1FF; // enable apic + set spurious to FF;
    apicconf->timer_divide_configuration.reg = 0x3; // divide by 16
    apicconf->timer_initial_count.reg = 0xFFFFFFFF; // count from -1
    uint64_t strt = get_current_tic();
    while(get_current_tic() < strt + 10)
    {

    }
    
}