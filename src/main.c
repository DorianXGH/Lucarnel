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
#include "tasks/context.h"
#include "tasks/tss.h"

uint8_t kernel_stack[0x4000] __attribute__((section(".stack"),used)) = {0};
struct stivale_header stivalehd __attribute__((section(".stivalehdr"),used)) = {
    .stack = (uintptr_t)&kernel_stack + sizeof(kernel_stack) - 1,
    .flags = 0x0001
};

struct stivale_struct stivale_global_info;
uint32_t procnum = 0;
extern volatile struct context next_task;
extern struct CR3 cr;

uint8_t init_task_stack[0x4000] = {0};

int init_system();

int main(struct stivale_struct *stivale_info)
{
    uint64_t * crview = (uint64_t *)(&cr);
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
    install_TSS();
    print("tss installed\n");
    _lgdt(&gdtd);
    print("gdt initialized\n");

    paging_init_identity();
    
    print("paging initialized\n");
   // print("512 first G identity mapped\n");
    print("cr3 ");
    print_num(*crview);
    print("\n");
    stivale_global_info = *stivale_info;

    // ----------------------- //
    // Initializing Interrupts //
    // ----------------------- //

    init_IDT();
    print("cr3 ");
    print_num(*crview);
    print("\n");
    // print("idt initialized\n");

    // ------------------------- //
    // Physical Memory Allocator //
    // ------------------------- //

    register_mmap((struct mmap_entry *) stivale_info->memory_map_addr,stivale_info->memory_map_entries);
    print("physical memory allocator initialized\n");
    //while (1) {}
    print("cr3 ");
    print_num(*crview);
    print("\n");

    // -------------------- //
    // Retrieving ACPI info //
    // -------------------- //
    
    init_kernel_acpi((struct RSDP2*)stivale_info->rsdp);
    
    print("acpi tables parsed\n");

    //init_system();
    next_task.regs.rax = 0;
    next_task.regs.rbx = 0;
    next_task.regs.rcx = 0;
    next_task.regs.rdx = 0;

    next_task.regs.rdi = 0;
    next_task.regs.rsi = 0;

    next_task.regs.r8 = 0;
    next_task.regs.r9 = 0;
    next_task.regs.r10 = 0;
    next_task.regs.r11 = 0;
    next_task.regs.r12 = 0;
    next_task.regs.r13 = 0;
    next_task.regs.r14 = 0;
    next_task.regs.r15 = 0;

    next_task.regs.frame.RIP = (uintptr_t)init_system;
    next_task.regs.frame.RSP = (uintptr_t)(init_task_stack + sizeof(init_task_stack));
    next_task.regs.rbp = (uintptr_t)(init_task_stack + sizeof(init_task_stack));
    next_task.regs.frame.CS = 0x18;
    next_task.regs.frame.SS = 0x20;
    next_task.cr = cr;
    print("cr3 ");
    print_num(*crview);
    print("\n");

    next_task.regs.error = 1;
    while (1) {}
}

int init_system()
{
    next_task.regs.error = 0;
    procnum++;
    print("proc ");
    print_num(procnum);
    print("\n");
    show_map();
    uint64_t addr = 0;
    for(uint8_t it = 0; it < 20; it++)
    {
        addr = (uint64_t) pmalloc(0x1000);
        print("allocated addr ");
        print_num(addr);
        print("\n");
        while(get_current_tick()%1000 != 0) {

        }
    }
    //show_map(); 
    while (1) {}
}