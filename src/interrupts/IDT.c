#include "../includes/interrupts.h"
#include "../includes/x86_64_utils.h"
#include <stdint.h>
#define IDT_SIZE 256

struct IDTE IDT[256];
struct IDTD idtd;

struct IDTE gen_IDT_entry(uintptr_t handler)
{
    return (struct IDTE){ // div by 0
        handler & 0xFFFF,
        0x08,       // kernel CS
        0,          // no IST
        0b10001110, // present - kernel - not storage - 64 bit interrupt
        (handler >> 16) & 0xFFFF,
        (handler >> 32) & 0xFFFFFFFF,
        0,
    };
}

void init_IDT()
{
    IDT[0] = gen_IDT_entry((uintptr_t)ISR_div_by_zero_handler); // div by 0
    IDT[1] = gen_IDT_entry((uintptr_t)ISR_debug_handler); // debug
    IDT[2] = gen_IDT_entry((uintptr_t)ISR_NMI_handler); // NMI
    IDT[3] = gen_IDT_entry((uintptr_t)ISR_breakpoint_handler);
    IDT[4] = gen_IDT_entry((uintptr_t)ISR_overflow_handler);
    IDT[5] = gen_IDT_entry((uintptr_t)ISR_bound_handler);
    IDT[6] = gen_IDT_entry((uintptr_t)ISR_invalid_opcode_handler);
    IDT[7] = gen_IDT_entry((uintptr_t)ISR_device_not_available_handler);
    IDT[8] = gen_IDT_entry((uintptr_t)ISR_double_fault_handler);
    IDT[9] = gen_IDT_entry((uintptr_t)ISR_coproc_segment_overrun_handler);
    IDT[10] = gen_IDT_entry((uintptr_t)ISR_invalid_TSS_handler);
    IDT[11] = gen_IDT_entry((uintptr_t)ISR_segment_not_present_handler);
    IDT[12] = gen_IDT_entry((uintptr_t)ISR_stack_segment_fault_handler);
    IDT[13] = gen_IDT_entry((uintptr_t)ISR_general_protection_fault_handler);
    IDT[14] = gen_IDT_entry((uintptr_t)ISR_page_fault_handler);
    IDT[32] = gen_IDT_entry((uintptr_t)IRQ_timer);
    for(int i = 15; i < 32; i++)
    {
        IDT[i] = gen_IDT_entry((uintptr_t)ISR_spurious);
    }
    for(int i = 33; i < 255; i++)
    {
        IDT[i] = gen_IDT_entry((uintptr_t)ISR_spurious);
    }
    IDT[255] = gen_IDT_entry((uintptr_t)ISR_spurious);
    idtd = (struct IDTD) {
        (IDT_SIZE*16)-1,
        (uintptr_t)&IDT
    };
    _lidt(&idtd);
}