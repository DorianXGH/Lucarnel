#include "../includes/interrupts.h"
#include "../includes/apic.h"
#include "../includes/stivale.h"
#include "../video/video.h"
#include "../includes/x86_64_utils.h"
#include "../includes/utils.h"
#include "../tasks/regfile.h"

uint64_t tick = 0;
uint64_t stick = 0;
uint8_t tickstr[19];

extern struct APICConfig* lapic_regs;

__attribute__((always_inline)) static inline void pushregs() {
    __asm__ volatile ("pushq 0\n"
    "pushq %rax\n"
    "pushq %rbx\n"
    "pushq %rcx\n"
    "pushq %rdx\n"
    "pushq %rdi\n"
    "pushq %rsi\n"
    "pushq %rbp\n"
    "pushq %r8\n"
    "pushq %r9\n"
    "pushq %r10\n"
    "pushq %r11\n"
    "pushq %r12\n"
    "pushq %r13\n"
    "pushq %r14\n"
    "pushq %r15\n"
    "movq %rdi, %rsp");
}
__attribute__((always_inline)) static inline void popregs() {
    __asm__ volatile ("popq %r15\n"
    "popq %r14\n"
    "popq %r13\n"
    "popq %r12\n"
    "popq %r11\n"
    "popq %r10\n"
    "popq %r9\n"
    "popq %r8\n"
    "popq %rbp\n"
    "popq %rsi\n"
    "popq %rdi\n"
    "popq %rdx\n"
    "popq %rcx\n"
    "popq %rbx\n"
    "popq %rax\n"
    "addq %rsp, 8\n"
    "iretq");
}

__attribute__((naked)) void IRQ_timer(struct regfile * regs) { //struct IFrame* interrupt_frame
    pushregs();
    tick++;
    if(tick%1000 == 0) {
        putString("tick",0,0,0xFFFFFFFF,0xFFFF0000,1);
        itohex(stick,tickstr);
        putString(tickstr,40,0,0xFFFFFFFF,0xFFFF0000,1);
        stick++;
    }
    outb(0x20, 0x20);
    lapic_regs->end_of_interrupt.reg = 0;
    popregs();
}

uint64_t get_current_tick()
{
    return tick;
}