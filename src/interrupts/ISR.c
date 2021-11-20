#include "../includes/interrupts.h"
#include "../includes/stivale.h"
#include "../video/video.h"

__attribute__((interrupt)) void ISR_general_handler(struct IFrame* interrupt_frame) {
    putString("isr",0,0,0xFFFFFFFF,0xFFFF0000,1);
}
__attribute__((interrupt)) void ISR_error_handler(struct IFrame* interrupt_frame, uint64_t error_code) {
    putString("isrerr",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_div_by_zero_handler(struct IFrame* interrupt_frame) {
    putString("divbyzero",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_debug_handler(struct IFrame* interrupt_frame) {
    putString("debug",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_NMI_handler(struct IFrame* interrupt_frame) {
    putString("nmi",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_breakpoint_handler(struct IFrame* interrupt_frame) {
    putString("breakpoint",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_overflow_handler(struct IFrame* interrupt_frame) {
    putString("overflow",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_bound_handler(struct IFrame* interrupt_frame) {
    putString("bound",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_invalid_opcode_handler(struct IFrame* interrupt_frame) {
    putString("invopcode",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_device_not_available_handler(struct IFrame* interrupt_frame) {
    putString("devnotavail",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_double_fault_handler(struct IFrame* interrupt_frame, uint64_t error_code) {
    putString("doublefault",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_coproc_segment_overrun_handler(struct IFrame* interrupt_frame) {
    putString("coprocsegover",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_invalid_TSS_handler(struct IFrame* interrupt_frame, uint64_t error_code) {
    putString("invtss",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_segment_not_present_handler(struct IFrame* interrupt_frame, uint64_t error_code) {
    putString("segnotpres",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_stack_segment_fault_handler(struct IFrame* interrupt_frame, uint64_t error_code) {
    putString("stacksegfault",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_general_protection_fault_handler(struct IFrame* interrupt_frame, uint64_t error_code) {
    putString("genprotecfault",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_page_fault_handler(struct IFrame* interrupt_frame, uint64_t error_code) {
    putString("pagefault",0,0,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_spurious(struct IFrame* interrupt_frame) {
    putString("spurious",0,0,0xFFFFFFFF,0xFFFF0000,1);
}
