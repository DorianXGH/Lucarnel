#include "tss.h"
#include "../includes/memory_structs.h"

extern struct GDTE gdt[];
struct TSS system_tss;
extern uint8_t kernel_stack[0x1000];

struct GDTE_TSS gdte_tss = {
        0x0067, // limit 1
        0, // base 1
        0, // base 2
        1, // accessed
        0, // read_write busy bit
        0, // direction conforming
        1, // code
        0, // system
        0, // ring
        1, // present
        0x0, // limit2
        0, // nullbits
        0, // granularity
        0, // base 3
        0, // base 4
        0  // reserved
};

void install_TSS(){
    gdte_tss.base1 = ((uintptr_t)(&system_tss))&0xFFFF;
    gdte_tss.base2 = ((uintptr_t)(&system_tss) >> 16)&0xFF;
    gdte_tss.base3 = ((uintptr_t)(&system_tss) >> 24)&0xFF;
    gdte_tss.base4 = ((uintptr_t)(&system_tss) >> 32)&0xFFFFFFFF;
    system_tss.iopb = sizeof(struct TSS);
    system_tss.rsp[0] = (((uintptr_t)kernel_stack) + sizeof(kernel_stack) - 1);
    struct GDTE_TSS* gdte = (struct GDTE_TSS*)(&(gdt[5]));
    *gdte = gdte_tss;
}