#include <stdint.h>
#include "includes/stivale.h"
#include "includes/memory_structs.h"
#include "video/video.h"
#include "memory/gdt.h"
#include "includes/x86_64_utils.h"
#include "memory/paging.h"
#include "includes/interrupts.h"
#include "includes/utils.h"
#include "includes/acpi.h"

uint8_t kernel_stack[0x1000] __attribute__((section(".stack"),used)) = {0};
struct stivale_header stivalehd __attribute__((section(".stivalehdr"),used)) = {
    .stack = (uintptr_t)&kernel_stack + sizeof(kernel_stack) - 1,
    .flags = 0x0001
};

struct stivale_struct stivale_global_info;
uint32_t procnum = 0;
int init_system();

int main(struct stivale_struct *stivale_info)
{
    // --------------------------- //
    // Initializing Video feedback //
    // --------------------------- //
    
    uint32_t *VMEM = (uint32_t *)(stivale_info->framebuffer_addr);
    VMEM[0] = 0xFF00FF00;
    VMEM[1] = 0xFF00FF00;
    VMEM[2] = 0xFF00FF00;
    VMEM[3] = 0xFF00FF00;

    initCharMap();
    register_framebuffer(stivale_info);

    clear();

    // ------------------------------ //
    // Initializing Memory Protection //
    // ------------------------------ //

    _lgdt(&gdtd);
    putString("gdt\0",0,64,0x00FF00FF,0xFF000000,2);

    paging_init_identity();
    putString("paging\0",0,128,0x00FF00FF,0xFF000000,2);

    stivale_global_info = *stivale_info;

    // ----------------------- //
    // Initializing Interrupts //
    // ----------------------- //

    init_IDT();
    putString("idt\0",0,128+64,0x00FF00FF,0xFF000000,2);

    // -------------------- //
    // Retrieving ACPI info //
    // -------------------- //
    
    init_kernel_acpi((struct RSDP2*)stivale_info->rsdp);
    
    putString("parsed\0",300,0,0x00FF00FF,0xFF000000,2);

    init_system();
}

int init_system()
{
    procnum++;
    putString("pric\0",200,128+64,0x00FF00FF,0xFF000000,2);
    uint8_t procnumstr[19];
    itohex(procnum,procnumstr);
    putString(procnumstr,300,128+64,0x00FF00FF,0xFF000000,2);
    while (1) {}
}