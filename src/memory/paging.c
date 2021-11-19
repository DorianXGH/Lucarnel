#include "paging.h"

struct PML4E pml4[512] __attribute__ ((aligned (4096)));
struct PDPTE pdpt[512] __attribute__ ((aligned (4096)));

void paging_init_identity()
{
    for(int i = 0; i < 512; i++)
    {
        pdpt[i] = (struct PDPTE){
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
        (uintptr_t)pdpt >> 12, // address of page
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
    struct CR3 cr3 = {
        0, // PCID or flags
        (uintptr_t)pml4 >> 12,
        0 // nullbits
    };
    _lcr3(&cr3);
}

void map(uint64_t vbase, uint64_t pbase, uint64_t size, bool cache_dis, bool code, bool supervisor)
{
    uint64_t i = 0;
    while ((i < size)) {
        map_page(vbase + i, pbase + i, cache_dis, code, supervisor, PS_4K);
        i += 0x1000;
    }
}
void map_page(uint64_t vp, uint64_t pp, bool cache_dis, bool code, bool supervisor, enum PAGE_SIZE sz)
{
    uint64_t pt_index = vp >> (12);
    uint64_t pd_index = vp >> (12+9);
    uint64_t pdpt_index = vp >> (12+9+9);
    uint64_t pml4_index = vp >> (12+9+9+9);
    pt_index &= 0x1ff;
    pd_index &= 0x1ff;
    pdpt_index &= 0x1ff;
    pml4_index &= 0x1ff;

    struct PDPTE * pdptl;
    struct PDE   * pd;
    struct PTE   * pt;

    if (pml4[pml4_index].present)
    {
        pdptl = (struct PDPTE *)(uintptr_t)(pml4[pml4_index].address << 12);
    } else {
        pdptl = (struct PDPTE *)pmalloc(0x1000);
        for(int i = 0; i < 512; i++)
        {
            pdptl[i] = (struct PDPTE){
                0, // NOT present
                0, // read and write
                0, // supervisor
                0, // write back
                0, // enable cache
                0, // not accessed (CPU set)
                0, // 1G
                0, // nullbits
                0, // address of page
                0, // nullbits
                0  // allow execution
            };
        }
        pml4[pml4_index] = (struct PML4E){
            1, // present
            1, // read and write
            0, // supervisor
            0, // write back
            0, // enable cache
            0, // not accessed (CPU set)
            0, // ignored
            0, // nullbits
            (uintptr_t)pdptl >> 12, // address of page
            0, // nullbits
            0  // allow execution
        };
    }

    if (pdptl[pdpt_index].present)
    {
        pd = (struct PDE *)(uintptr_t)(pdptl[pdpt_index].address << 12);
    } else {
        if (sz == PS_1G)
        {
            pdptl[pdpt_index] = (struct PDPTE) {
                1, // present
                !code,
                supervisor,
                cache_dis,
                cache_dis, 
                0, // not accessed (CPU set)
                1, // 1G
                0, // nullbits
                pp >> 12, // address of page
                0, // nullbits
                !code
            };
        } else 
        {
            pd = (struct PDE *)pmalloc(0x1000);
            for(int i = 0; i < 512; i++)
            {
                pd[i] = (struct PDE){
                    0, // NOT present
                    0, // read and write
                    0, // supervisor
                    0, // write back
                    0, // enable cache
                    0, // not accessed (CPU set)
                    0, // 2M
                    0, // nullbits
                    0, // address of page
                    0, // nullbits
                    0  // allow execution
                };
            }
            pdptl[pdpt_index] = (struct PDPTE){
                1, // present
                1, // read and write
                0, // supervisor
                0, // write back
                0, // enable cache
                0, // not accessed (CPU set)
                0, // 1G
                0, // nullbits
                (uintptr_t)pd >> 12, // address of page
                0, // nullbits
                0  // allow execution
            };
        }
    }

    if (pd[pd_index].present)
    {
        pt = (struct PTE *)(uintptr_t)(pd[pd_index].address << 12);
    } else {
        if (sz == PS_2M)
        {
            pd[pd_index] = (struct PDE) {
                1, // present
                !code,
                supervisor,
                cache_dis,
                cache_dis, 
                0, // not accessed (CPU set)
                1, // 2M
                0, // nullbits
                pp >> 12, // address of page
                0, // nullbits
                !code
            };
        } else 
        {
            pt = (struct PTE *)pmalloc(0x1000);
            for(int i = 0; i < 512; i++)
            {
                pt[i] = (struct PTE){
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
            pd[pd_index] = (struct PDE){
                1, // present
                1, // read and write
                0, // supervisor
                0, // write back
                0, // enable cache
                0, // not accessed (CPU set)
                0, // 2M
                0, // nullbits
                (uintptr_t)pt >> 12, // address of page
                0, // nullbits
                0  // allow execution
            };
        }
    }

    if (! (pt[pt_index].present) )
    {
        pt[pt_index] = (struct PTE) {
                1, // present
                !code,
                supervisor,
                cache_dis,
                cache_dis, 
                0, // not accessed (CPU set)
                0, // ignored
                0, // nullbits
                pp >> 12, // address of page
                0, // nullbits
                !code
            };
    }
}