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
#include "smp/smp.h"

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
    uint32_t *VMEM = (uint32_t *)(stivale_info->framebuffer_addr);
    VMEM[0] = 0xFF00FF00;
    VMEM[1] = 0xFF00FF00;
    VMEM[2] = 0xFF00FF00;
    VMEM[3] = 0xFF00FF00;
    initCharMap();
    putChar(' ',0,20,stivale_info,0xFF000000,0xFF000000,2);
    putChar(' ',16,20,stivale_info,0x00FF0000,0x00FF0000,2);
    putChar(' ',32,20,stivale_info,0x0000FF00,0x0000FF00,2);
    putChar(' ',48,20,stivale_info,0x000000FF,0x000000FF,2);
    for(char i = 'a'; i <= 'z'; i++)
    {
        putChar(i,(i-'a')*16,256,stivale_info,0x00FFFFFF,0x00FF0000,2);
    }
    _lgdt(&gdtd);
    putString("gdt\0",0,64,stivale_info,0x00FF00FF,0xFF000000,2);
    paging_init_identity();
    putString("paging\0",0,128,stivale_info,0x00FF00FF,0xFF000000,2);

    stivale_global_info = *stivale_info;

    init_IDT();
    putString("idt\0",0,128+64,stivale_info,0x00FF00FF,0xFF000000,2);

    init_kernel_acpi((struct RSDP2*)stivale_info->rsdp);

    putString("parsed\0",300,0,&stivale_global_info,0x00FF00FF,0xFF000000,2);
    smp_bootstrap_install();
    putString("btstrp\0",300,0,&stivale_global_info,0x00FF00FF,0xFF000000,2);
    init_system();
}

int init_system()
{
    procnum++;
    putString("proc\0",200,128+64,&stivale_global_info,0x00FF00FF,0xFF000000,2);
    uint8_t procnumstr[19];
    itohex(procnum,procnumstr);
    putString(procnumstr,300,128+64,&stivale_global_info,0x00FF00FF,0xFF000000,2);
}