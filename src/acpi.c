#include "includes/acpi.h"
#include "includes/stivale.h"
#include "video/video.h"
#include "includes/utils.h"
#include "includes/apic.h"
#include "includes/pci.h"

struct ACPISDTHeader* xsdt;
struct ACPISDTHeader* madt = 0;
struct ACPISDTHeader* mcfg = 0;

// --------------------------- //
// Check ACPI header integrity //
// --------------------------- //

bool do_checksum(struct ACPISDTHeader* table_header)
{
    uint8_t sum = 0;
    for (int i = 0; i < table_header->length; i++)
    {
        sum += ((uint8_t*)table_header)[i];
    }
    return sum == 0;
}

// ------------------------------------------------------- //
// Retrieves and initializes ACPI related systems and info //
// ------------------------------------------------------- //

void init_kernel_acpi(struct RSDP2* rsdp)
{
    // ----------------- //
    // Check ACPI header //
    // ----------------- //

    if(rsdp->header.revision != 2)
    {
        putString("ACPI 2 or more needed\0",0,10,0xFFFFFFFF,0x00FF0000,1);
    }

    if(arrcmp(rsdp->header.signature,"RSD PTR ",8))
    {
        putString("RSDP Valid\0",0,20,0xFFFFFFFF,0x00FF0000,1);
    }

    // ------------------------------ //
    // Retrieve XSDT and check header //
    // ------------------------------ //

    uint8_t xsdtaddr[19];
    itohex(rsdp->xsdt_address,xsdtaddr);
    putString(xsdtaddr,0,30,0xFF000000,0x00FFFFFF,1);

    xsdt = (struct ACPISDTHeader*)(rsdp->xsdt_address);

    if(do_checksum(xsdt) && arrcmp(xsdt->signature,"XSDT",4))
    {
        putString("XSDT valid\0",0,40,0xFFFFFFFF,0x00FF0000,1);
    }

    // --------------------- //
    // Retrieve other tables //
    // --------------------- //

    uint64_t entries = (xsdt->length - sizeof(struct ACPISDTHeader)) / 8;
    uint8_t numentries[19];
    itohex(entries,numentries);
    putString("num \0",0,50,0xFFFFFFFF,0x00FF0000,1);
    putString(numentries,32,50,0xFFFFFFFF,0x00FF0000,1);

    uint64_t* other_tables = (uint64_t*)((uintptr_t)xsdt + sizeof(struct ACPISDTHeader));
    uint8_t sig[5];
    sig[4] = 0;
    uint8_t addrentry[19];
    itohex((uint64_t)other_tables,addrentry);
    putString(addrentry,8*40,60,0xFFFFFFFF,0x00FF0000,1);

    for(int i = 0; i < entries; i++)
    {
        struct ACPISDTHeader* entry = (struct ACPISDTHeader*)(other_tables[i]);
        
        for(int k = 0; k < 4; k++)
        {
            sig[k] = (entry->signature)[k];
        }
        putString(sig,0,60+10*i,0xFFFFFFFF,0x00FF0000,1);
        
        // --------- //
        // Find MADT //
        // --------- //

        if(arrcmp(sig,"APIC",4))
        {
            madt = entry;
            putString("apic",8*30,60+10*i,0xFFFFFFFF,0x000000FF,1);
        }
        if(arrcmp(sig,"MCFG",4))
            mcfg = entry;
            putString("pci",8*30,60+10*i,0xFFFFFFFF,0x000000FF,1);
            
        
        itohex(other_tables[i],addrentry);
        putString(addrentry,8*5,60+10*i,0xFFFFFFFF,0x00FF0000,1);
    }
    if(madt != 0)
    {
        parse_madt();
    }
    if(mcfg != 0)
    {
        init_pci_tree((struct MCFG*)mcfg);
    }
}

// ----------- //
// Parses MADT //
// ----------- //

void write_ioapic_register(const uintptr_t apic_base, const uint8_t offset, const uint32_t val) 
{
    /* tell IOREGSEL where we want to write to */
    *(volatile uint32_t*)(apic_base) = offset;
    /* write the value to IOWIN */
    *(volatile uint32_t*)(apic_base + 0x10) = val; 
}
 
uint32_t read_ioapic_register(const uintptr_t apic_base, const uint8_t offset)
{
    /* tell IOREGSEL where we want to read from */
    *(volatile uint32_t*)(apic_base) = offset;
    /* return the data from IOWIN */
    return *(volatile uint32_t*)(apic_base + 0x10);
}

void parse_madt()
{
    // ------------------- //
    // Get local APIC info //
    // ------------------- //

    uintptr_t max = (uintptr_t)madt + (madt->length);
    uintptr_t current = (uintptr_t)madt + sizeof(struct ACPISDTHeader);
    struct MADTLocal_APIC* lapic = (struct MADTLocal_APIC*)current;
    uint8_t addrapic[19];
    itohex(lapic->address,addrapic);
    putString("loc\0",0,200,0xFFFFFFFF,0x000000FF,1);
    putString(addrapic,0,210,0xFFFFFFFF,0x000000FF,1);
    uint8_t flagsapic[19];
    itohex(lapic->flags,flagsapic);
    putString(flagsapic,8*20,210,0xFFFFFFFF,0x000000FF,1);
    current += sizeof(struct MADTLocal_APIC);

    // --------------- //
    // Configure LAPIC //
    // --------------- //

    struct APICConfig* local_apic_config = (struct APICConfig*)((uintptr_t)(lapic->address));
    

    // ------------------- //
    // Get the other APICs //
    // ------------------- //

    int i = 0;
    while (current < max) // for each entry
    {
        struct MADTEntry_header* entry = ((struct MADTEntry_header*)(current));
        if(entry->entry_type == 0)
        {
            struct LAPIC* apic = (struct LAPIC*)entry;
            uint8_t procid[19];
            itohex(apic->processorID,procid);
            uint8_t apicid[19];
            itohex(apic->LAPIC_ID,apicid);
            uint8_t flags[19];
            itohex(apic->flags,flags);
            putString(procid,0,220+10*i,0xFFFFFFFF,0x000000FF,1);
            putString(apicid,8*20,220+10*i,0xFFFFFFFF,0x000000FF,1);
            putString(flags,8*40,220+10*i,0xFFFFFFFF,0x000000FF,1);
        }
        if(entry->entry_type == 1)
        {
            struct IOAPIC* apic = (struct IOAPIC*)entry;
            uint8_t id[19];
            itohex(apic->IOAPIC_ID,id);
            uint8_t gsi[19];
            itohex(apic->GSI_base,gsi);
            uint8_t addrmmio[19];
            itohex(apic->address,addrmmio);
            putString(id,0,220+10*i,0xFFFFFFFF,0x000000FF,1);
            putString(gsi,8*20,220+10*i,0xFFFFFFFF,0x000000FF,1);
            putString(addrmmio,8*40,220+10*i,0xFFFFFFFF,0x000000FF,1);
            if(apic->GSI_base == 0)
            {
                write_ioapic_register(apic->address,0x10,0x00000020);
                write_ioapic_register(apic->address,0x11,(local_apic_config->LAPIC_ID.reg & 0xF)<<24);
            }
        }
        i++;
        if(entry->length == 0)
        {
            break;
        }
        current += entry->length;

        putString("acpi\0",300,0,0x00FF0000,0xFF000000,1);
        uint8_t acpientryparsed[19];
        itohex(i,acpientryparsed);
        putString(acpientryparsed,350,0,0x00FF0000,0xFF000000,1);
        itohex(current,acpientryparsed);
        putString(acpientryparsed,350,10,0x00FF0000,0xFF000000,1);
        itohex(max,acpientryparsed);
        putString(acpientryparsed,350,20,0x00FF0000,0xFF000000,1);
    }
    putString("init lapic\0",0,200,0xFFFFFFFF,0x000000FF,1);
    init_lapic(local_apic_config);
    putString("init lapic done\0",0,200,0xFFFFFFFF,0x000000FF,1);
}