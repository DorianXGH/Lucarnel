#include "includes/acpi.h"
#include "includes/stivale.h"
#include "video/video.h"
#include "includes/utils.h"
#include "includes/apic.h"
#include "includes/pci.h"
#include "video/cons.h"

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
        print("ACPI 2 or more needed\n");
    }

    if(arrcmp(rsdp->header.signature,"RSD PTR ",8))
    {
        print("RSDP Valid\n");
    }

    // ------------------------------ //
    // Retrieve XSDT and check header //
    // ------------------------------ //
    print("xsdt ");
    print_num(rsdp->xsdt_address);
    print("\n");

    xsdt = (struct ACPISDTHeader*)(rsdp->xsdt_address);

    if(do_checksum(xsdt) && arrcmp(xsdt->signature,"XSDT",4))
    {
        print("XSDT valid\n");
    }

    // --------------------- //
    // Retrieve other tables //
    // --------------------- //

    uint64_t entries = (xsdt->length - sizeof(struct ACPISDTHeader)) / 8;
    print_num(entries);
    print(" entries\n");
    print("\n");

    uint64_t* other_tables = (uint64_t*)((uintptr_t)xsdt + sizeof(struct ACPISDTHeader));
    uint8_t sig[5];
    sig[4] = 0;

    for(int i = 0; i < entries; i++)
    {
        struct ACPISDTHeader* entry = (struct ACPISDTHeader*)(other_tables[i]);
        
        for(int k = 0; k < 4; k++)
        {
            sig[k] = (entry->signature)[k];
        }
        print(sig);
        print("   ");
        print_num(other_tables[i]);
        print("   ");
        // --------- //
        // Find MADT //
        // --------- //

        if(arrcmp(sig,"APIC",4))
        {
            madt = entry;
            print("   apic");
        }
        if(arrcmp(sig,"MCFG",4))
        {
            mcfg = entry;
            print("   pci");
        }
        
        print("\n");
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
    print("apic address ");
    print_num(lapic->address);
    print("\napic flags ");
    print_num(lapic->flags);
    print("\n");
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
            print("\nLAPIC\n\n");
            struct LAPIC* apic = (struct LAPIC*)entry;
            print("processor id ");
            print_num(apic->processorID);
            print("\napic id ");
            print_num(apic->LAPIC_ID);
            print("\napic flags ");
            print_num(apic->flags);
            print("\n");
        }
        if(entry->entry_type == 1)
        {
            print("\nIOAPIC\n\n");
            struct IOAPIC* apic = (struct IOAPIC*)entry;

            print("ioapic id ");
            print_num(apic->IOAPIC_ID);
            print("\ngsi base ");
            print_num(apic->GSI_base);
            print("\nmmio address ");
            print_num(apic->address);
            print("\n");

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
    }
    
    init_lapic(local_apic_config);
    print("\ninit lapic done\n\n");
}