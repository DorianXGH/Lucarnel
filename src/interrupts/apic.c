#include "../includes/apic.h"
#include "../includes/stivale.h"
#include "../video/video.h"

extern struct stivale_struct stivale_global_info;
struct APICConfig* lapic_regs;

void pit_wait(uint16_t ms) {
    outb(0x43, 0x30);     
    uint16_t val = 0x4a9 * ms;     
    outb(0x40, (uint8_t)val); 
    outb(0x40, (uint8_t)(val >> 8));     
    while (true) {         
        outb(0x43, 0xe2);         
        uint8_t status = inb(0x40);         
        if ((status & (1 << 7)) != 0) {             
            break;         
        }     
    } 
}

void init_lapic(struct APICConfig* apicconf)
{
    lapic_regs = apicconf;
    apicconf->spurious_interrupt_vector.reg = 0x0FF; // disable apic + set spurious to FF;
    apicconf->timer_divide_configuration.reg = 0x3; // divide by 16
    apicconf->timer_initial_count.reg = 0xFFFFFFFF; // count from -1
    pit_wait(1);
    apicconf->timer_initial_count.reg = 0xFFFFFFFF - apicconf->timer_current_count.reg;
    apicconf->LVT_timer.reg = 0x20020;
    apicconf->spurious_interrupt_vector.reg = 0x1FF; // enable apic + set spurious to FF;
    __asm__ volatile ("sti");
}