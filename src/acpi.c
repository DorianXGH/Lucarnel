#include "includes/acpi.h"
#include "includes/stivale.h"
#include "video/video.h"
#include "includes/utils.h"

extern struct stivale_struct stivale_global_info;

struct ACPISDTHeader* xsdt;
struct ACPISDTHeader* madt = 0;

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

    uint64_t entries = (xsdt->length - sizeof(struct ACPISDTHeader)) / 8;
    uint8_t numentries[19];
    itohex(entries,numentries);
    putString("num \0",0,50,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
    putString(numentries,32,50,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);

    uint64_t* other_tables = (uintptr_t)xsdt + sizeof(struct ACPISDTHeader);
    uint8_t sig[5];
    sig[4] = 0;
    uint8_t addrentry[19];
    itohex(other_tables,addrentry);
    putString(addrentry,8*40,60,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);

    for(int i = 0; i < entries; i++)
    {
        struct ACPISDTHeader* entry = (struct ACPISDTHeader*)(other_tables[i]);
        
        for(int k = 0; k < 4; k++)
        {
            sig[k] = (entry->signature)[k];
        }
        putString(sig,0,60+10*i,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
        
        if(arrcmp(sig,"APIC",4))
        {
            madt = entry;
            putString("apic",8*30,60+10*i,&stivale_global_info,0xFFFFFFFF,0x000000FF,1);
        }
        
        itohex(other_tables[i],addrentry);
        putString(addrentry,8*5,60+10*i,&stivale_global_info,0xFFFFFFFF,0x00FF0000,1);
    }
    if(madt != 0)
    {
        parse_madt();
    }
}

void parse_madt()
{
    uintptr_t max = madt + (madt->length);
    uintptr_t current = madt + sizeof(struct ACPISDTHeader);
    struct MADTLocal_APIC* lapic = current;
    uint8_t addrapic[19];
    itohex(lapic->address,addrapic);
    putString("loc\0",0,200,&stivale_global_info,0xFFFFFFFF,0x000000FF,1);
    putString(addrapic,0,210,&stivale_global_info,0xFFFFFFFF,0x000000FF,1);
    uint8_t flagsapic[19];
    itohex(lapic->flags,flagsapic);
    putString(flagsapic,8*20,210,&stivale_global_info,0xFFFFFFFF,0x000000FF,1);
}