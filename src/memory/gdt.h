#pragma once
#include "../includes/memory_structs.h"
#include "../tasks/tss.h"

struct GDTE gdt[] = {
    { // null segment
        0, // limit 1
        0, // base 1
        0, // base 2
        0, // accessed
        0, // read_write
        0, // direction conforming
        0, // code
        0, // descriptor
        0, // ring
        0, // present
        0, // limit2
        0, // nullbits
        0, // longmode
        0, // 32 bit protected
        0, // limit using pages
        0  // base 3
    },
    { // kernel code segment
        0xFFFF, // limit 1
        0, // base 1
        0, // base 2
        0, // accessed
        0, // read_write can't write to code
        0, // direction conforming
        1, // code
        1, // descriptor
        0, // ring
        1, // present
        0xF, // limit2
        0, // nullbits
        1, // longmode
        0, // 32 bit protected
        1, // limit using pages
        0  // base 3
    },
    { // kernel data segment
        0xFFFF, // limit 1
        0, // base 1
        0, // base 2
        0, // accessed
        1, // read_write
        0, // direction conforming
        0, // code
        1, // descriptor
        0, // ring
        1, // present
        0xF, // limit2
        0, // nullbits
        0, // longmode
        1, // 32 bit protected
        1, // limit using pages
        0  // base 3
    },
    { // user code segment
        0xFFFF, // limit 1
        0, // base 1
        0, // base 2
        0, // accessed
        0, // read_write can't write to code
        0, // direction conforming
        1, // code
        1, // descriptor
        3, // ring
        1, // present
        0xF, // limit2
        0, // nullbits
        1, // longmode
        0, // 32 bit protected
        1, // limit using pages
        0  // base 3
    },
    { // user data segment
        0xFFFF, // limit 1
        0, // base 1
        0, // base 2
        0, // accessed
        1, // read_write
        0, // direction conforming
        0, // code
        1, // descriptor
        3, // ring
        1, // present
        0xF, // limit2
        0, // nullbits
        0, // longmode
        1, // 32 bit protected
        1, // limit using pages
        0  // base 3
    },
    { // reserve space for TSS
        0, // limit 1
        0, // base 1
        0, // base 2
        0, // accessed
        0, // read_write
        0, // direction conforming
        0, // code
        0, // descriptor
        0, // ring
        0, // present
        0, // limit2
        0, // nullbits
        0, // longmode
        0, // 32 bit protected
        0, // limit using pages
        0  // base 3
    },
    { //
        0, // limit 1
        0, // base 1
        0, // base 2
        0, // accessed
        0, // read_write
        0, // direction conforming
        0, // code
        0, // descriptor
        0, // ring
        0, // present
        0, // limit2
        0, // nullbits
        0, // longmode
        0, // 32 bit protected
        0, // limit using pages
        0  // base 3
    }
};

struct GDTD gdtd = {
    sizeof(gdt)-1,
    (uintptr_t)gdt
};