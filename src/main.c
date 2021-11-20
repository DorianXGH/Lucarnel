#include <stdint.h>
#include "includes/stivale.h"
#include "includes/memory_structs.h"
#include "video/video.h"
#include "video/cons.h"
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
    print("gdt initialized\n");

    paging_init_identity();
    print("paging initialized\n");
    print("512 first G identity mapped\n");

    stivale_global_info = *stivale_info;

    // ----------------------- //
    // Initializing Interrupts //
    // ----------------------- //

    init_IDT();
    print("idt initialized\n");

    // ------------------------- //
    // Physical Memory Allocator //
    // ------------------------- //

    register_mmap((struct mmap_entry *) stivale_info->memory_map_addr,stivale_info->memory_map_entries);
    print("physical memory allocator initialized\n");

    // -------------------- //
    // Retrieving ACPI info //
    // -------------------- //
    
    init_kernel_acpi((struct RSDP2*)stivale_info->rsdp);
    
    print("acpi tables parsed\n");

    init_system();
}

int init_system()
{
    procnum++;
    print("proc ");
    print_num(procnum);
    print("\n");
    while (1) {}
}