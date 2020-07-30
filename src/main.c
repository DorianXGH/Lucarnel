#include <stdint.h>
#include "includes/stivale.h"
#include "includes/memory_structs.h"
#include "video/video.h"
#include "memory/gdt.h"
#include "includes/x86_64_utils.h"
#include "memory/paging.h"

uint8_t kernel_stack[0x1000] __attribute__((section(".stack"),used)) = {0};
struct stivale_header stivalehd __attribute__((section(".stivalehdr"),used)) = {
    .stack = (uintptr_t)&kernel_stack + sizeof(kernel_stack) - 1,
    .flags = 0x0001
};

int main(struct stivale_struct *stivale_info)
{
    uint32_t *VMEM = (uint32_t *)(stivale_info->framebuffer_addr);
    VMEM[0] = 0xFF00FF00;
    VMEM[1] = 0xFF00FF00;
    VMEM[2] = 0xFF00FF00;
    VMEM[3] = 0xFF00FF00;
    initCharMap();
    putChar(' ',20+0*16,20,stivale_info,0xFF000000,0xFF000000,2);
    putChar(' ',20+1*16,20,stivale_info,0x00FF0000,0x00FF0000,2);
    putChar(' ',20+2*16,20,stivale_info,0x0000FF00,0x0000FF00,2);
    putChar(' ',20+3*16,20,stivale_info,0x000000FF,0x000000FF,2);
    for(char i = 'a'; i <= 'l'; i++)
    {
        putChar(i,(i-'a')*24,48,stivale_info,0x00FFFFFF,0x00FF0000,3);
    }
    _lgdt(&gdtd);
    putChar(' ',20+0*16,64,stivale_info,0xFF000000,0xFF000000,2);
    putChar(' ',20+1*16,64,stivale_info,0x00FF0000,0x00FF0000,2);
    putChar(' ',20+2*16,64,stivale_info,0x0000FF00,0x0000FF00,2);
    putChar(' ',20+3*16,64,stivale_info,0x000000FF,0x000000FF,2);
    paging_init_identity();
    putChar('a',20+0*16,128,stivale_info,0xFF000000,0xFF000000,2);
    putChar('g',20+1*16,128,stivale_info,0x00FF0000,0x00FF0000,2);
    putChar('i',20+2*16,128,stivale_info,0x0000FF00,0x0000FF00,2);
    putChar('g',20+3*16,128,stivale_info,0x000000FF,0x000000FF,2);
}