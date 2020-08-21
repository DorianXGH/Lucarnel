#include "smp.h"
void smp_long_bootstrap()
{

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