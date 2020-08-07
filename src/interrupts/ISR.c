#include "../includes/interrupts.h"
#include "../includes/stivale.h"
#include "../video/video.h"

extern struct stivale_struct stivale_global_info;

__attribute__((interrupt)) void ISR_general_handler(struct IFrame* interrupt_frame) {
    putString("isr",0,0,&stivale_global_info,0xFFFFFFFF,0xFFFF0000,1);
}
__attribute__((interrupt)) void ISR_error_handler(struct IFrame* interrupt_frame, uint64_t error_code) {
    putString("isrerr",0,0,&stivale_global_info,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_div_by_zero_handler(struct IFrame* interrupt_frame) {
    putString("divbyzero",0,0,&stivale_global_info,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_debug_handler(struct IFrame* interrupt_frame) {
    putString("debug",0,0,&stivale_global_info,0xFFFFFFFF,0xFFFF0000,1);
}

__attribute__((interrupt)) void ISR_NMI_handler(struct IFrame* interrupt_frame) {
    putString("nmi",0,0,&stivale_global_info,0xFFFFFFFF,0xFFFF0000,1);
}