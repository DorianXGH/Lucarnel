#include "includes/acpi.h"
#include "includes/stivale.h"
#include "video/video.h"

extern struct stivale_struct stivale_global_info;

struct ACPISDTHeader* xsdt;
struct ACPISDTHeader* madt;

bool do_checksum(struct ACPISDTHeader* table_header)
{
    uint8_t sum = 0;
    for (int i = 0; i < table_header->length; i++)
    {
        sum += ((uint8_t*)table_header)[i];
    }
    return sum == 0;
}

void init_kernel_acpi(struct RSDP2* rsdp)
{
    uint8_t rsdpaddr[19];
    itohex(stivale_global_info.rsdp,rsdpaddr);
    putString(rsdpaddr,200,0,&stivale_global_info,0xFF000000,0x00FFFFFF,1);

    struct RSDP2* rsdp = (struct RSDP2*)(stivale_global_info.rsdp);

    if(rsdp->header.revision != 2)
    {
        putString("ACPI 2 or more needed\0",0,10,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
    }

    if(arrcmp(rsdp->header.signature,"RSD PTR ",8))
    {
        putString("RSDP Valid\0",0,20,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
    }

    uint8_t xsdtaddr[19];
    itohex(rsdp->xsdt_address,xsdtaddr);
    putString(xsdtaddr,0,30,&stivale_global_info,0xFF000000,0x00FFFFFF,1);

    xsdt = (struct ACPISDTHeader*)(rsdp->xsdt_address);

    if(do_checksum(xsdt) && arrcmp(xsdt->signature,"XSDT",4))
    {
        putString("XSDT valid\0",0,40,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
    }
}