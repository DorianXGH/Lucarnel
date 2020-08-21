#include "../includes/apic.h"
void init_lapic(struct APICConfig* apicconf)
{
    apicconf->spurious_interrupt_vector.reg = 0x1FF; // enable apic + set spurious to FF;
}