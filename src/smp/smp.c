#include "smp.h"

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
    for(uint8_t* ptr = smp_start; ptr < smp_end; ptr++)
    {
        *btstrp_ptr = *ptr;
        btstrp_ptr++;
    }
}