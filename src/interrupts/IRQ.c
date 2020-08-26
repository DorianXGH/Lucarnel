#include "../includes/interrupts.h"
uint64_t tick = 0;
void init_pit()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 40);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    outb(0x43, 0x36);

    uint32_t divisor = 0x1234dc / 1000; // 1 khz : 1ms
    uint8_t l = divisor & 0xff;
    uint8_t h = (divisor >> 8) & 0xff;

    outb(0x40, l);
    outb(0x40, h);
}

__attribute__((interrupt)) void IRQ_timer(struct IFrame* interrupt_frame) {
    tick++;
}

uint64_t get_current_tick()
{
    return tick;
}