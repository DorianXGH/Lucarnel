#include "paging.h"

void paging_init_identity()
{
    for(int i = 0; i < 512; i++)
    {
        pdpte[i] = (struct PDPTE){
            1, // present
            1, // Read/Write
            1, // Supervisor
            0, // write back
            0, // enable cache
            0, // accessed
            0, // dirty ! only if big page
            1, // 1G page
            0, // nullbits
            i << (30-12), // address (*1G) 
            0, // reserved nullbits
            0  // enables execution
        };
    }
    pml4[0] = (struct PML4E){
        1, // present
        1, // read and write
        1, // supervisor
        0, // write back
        0, // enable cache
        0, // not accessed (CPU set)
        0, // ignored
        0, // nullbits
        (uintptr_t)pdpte >> 12, // address of page
        0, // nullbits
        0  // allow execution
    };
    for(int i = 1; i < 512; i++)
    {
        pml4[i] = (struct PML4E){
            0, // NOT present
            0, // read and write
            0, // supervisor
            0, // write back
            0, // enable cache
            0, // not accessed (CPU set)
            0, // ignored
            0, // nullbits
            0, // address of page
            0, // nullbits
            0  // allow execution
        };
    }
}