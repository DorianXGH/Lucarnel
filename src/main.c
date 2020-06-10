#include <stdint.h>
#include "includes/stivale.h"
#include "includes/paging.h"

uint8_t kernel_stack[0x1000] __attribute__((section(".stack"),used)) = {0};
struct stivale_header stivalehd __attribute__((section(".stivalehdr"),used)) = {
    .stack = (uintptr_t)&kernel_stack + sizeof(kernel_stack),
    .flags = 0x0001
};

int main(struct stivale_struct *stivale_info)
{
    uint32_t *VMEM = (uint32_t *)(stivale_info->framebuffer_addr);
    VMEM[0] = 0xFF00FF00;
    VMEM[1] = 0xFF00FF00;
    VMEM[2] = 0xFF00FF00;
    VMEM[3] = 0xFF00FF00;
}