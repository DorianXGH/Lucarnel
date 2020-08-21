#include "smp.h"

extern struct stivale_struct stivale_global_info;

void smp_long_bootstrap()
{
    _lgdt(&gdtd);
    _lidt(&idtd);
    struct CR3 cr3 = {
        0, // PCID or flags
        (uintptr_t)pml4 >> 12,
        0 // nullbits
    };
    _lcr3(&cr3);
    init_system();
}

void smp_bootstrap_install()
{
    uint8_t* btstrp_ptr = 0x8000;
    uint8_t ptrstr[19];
    for(uint8_t* ptr = 0x001EF000; ptr < 0x001F0000; ptr++)
    {
        itohex(ptr,ptrstr);
        putString(ptrstr,200,50,&stivale_global_info,0x00FF00FF,0xFF000000,1);
        *btstrp_ptr = *ptr;
        btstrp_ptr++;
    }
}