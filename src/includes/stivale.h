#pragma once
#include <stdint.h>

struct stivale_header {
    uint64_t stack;   // This is the stack address which will be in RSP
                      // when the kernel is loaded.

    uint16_t flags;   // Flags
                      // bit 0  0 = text mode, 1 = graphics framebuffer mode
                      // bit 1  0 = 4-level paging, 1 = use 5-level paging (if
                      //                                  available)
                      //          Ignored if booting a 32-bit kernel.
                      // bit 2  0 = Disable KASLR, 1 = enable KASLR (up to 1GB slide)
                      //          Ignored if booting a 32-bit or non-relocatable kernel
                      // All other bits undefined.

    uint16_t framebuffer_width;   // These 3 values are parsed if a graphics mode
    uint16_t framebuffer_height;  // is requested. If all values are set to 0
    uint16_t framebuffer_bpp;     // then the bootloader will pick the best possible
                                  // video mode automatically (recommended).
    uint64_t entry_point;      // If not 0, this field will be jumped to at entry
                               // instead of the ELF entry point.
} __attribute__((packed));

struct stivale_struct {
    uint64_t cmdline;               // Pointer to a null-terminated cmdline
    uint64_t memory_map_addr;       // Pointer to the memory map (entries described below)
    uint64_t memory_map_entries;    // Count of memory map entries
    uint64_t framebuffer_addr;      // Address of the framebuffer and related info
    uint16_t framebuffer_pitch;
    uint16_t framebuffer_width;
    uint16_t framebuffer_height;
    uint16_t framebuffer_bpp;
    uint64_t rsdp;                  // Pointer to the ACPI RSDP structure
    uint64_t module_count;          // Count of modules that stivale loaded according to config
    uint64_t modules;               // Pointer to the first entry in the linked list of modules (described below)
    uint64_t epoch;                 // UNIX epoch at boot, read from system RTC
    uint64_t flags;                 // Flags
                                    // bit 0: 1 if booted with BIOS, 0 if booted with UEFI
                                    // All other bits undefined.
} __attribute__((packed));

struct mmap_entry {
    uint64_t base;      // Base of the memory section
    uint64_t length;    // Length of the section
    uint32_t type;      // Type (described below)
    uint32_t unused;
} __attribute__((packed));