#include "includes/pci.h"
#include "video/video.h"
#include "video/cons.h"
#include "memory/mmap.h"

void * driver_callbacks[256][256];

void init_pci_tree(struct MCFG* mcfg) 
{
    uint64_t num_entries = (mcfg->header.length - sizeof(mcfg->header) - sizeof(mcfg->reserved))/sizeof(struct MCFGEntry);
    for(uint64_t i = 0; i < num_entries; i++) {
        print("\npci segment group \n");
        print("   base ");
        print_num(mcfg->entries[i].base);
        print("\n");
        print("   segment group ");
        print_num(mcfg->entries[i].segment_group);
        print("\n");
        print("   bus start ");
        print_num(mcfg->entries[i].bus_start);
        print("\n");
        print("   bus end ");
        print_num(mcfg->entries[i].bus_end);
        print("\n");
        for(uint8_t bus = mcfg->entries[i].bus_start; bus <= mcfg->entries[i].bus_end; bus++)
        {
            for(uint8_t device = 0; device <= 255; device++)
            {
                struct pci_config_header_common * cfg = get_PCI_device_cfg_space(mcfg->entries[i].base, bus, device, 0);
                if(cfg->vendor_ID == 0xFFFF) // no device
                    break;
                else {
                    // bool (*callback)(struct pci_config_header_common *) = (bool (*)(struct pci_config_header_common *))(driver_callbacks[cfg->class][cfg->subclass]); WRONG, need one more level
                    // scan over functions
                }
            }
        }
    }
}

struct pci_config_header_common * get_PCI_device_cfg_space(uint64_t PCI_config_base, uint8_t bus, uint8_t device, uint8_t function)
{
    return (struct pci_config_header_common *)(PCI_config_base + (bus << 20 | device << 15 | function << 12));
}

void pci_device_callback(uint8_t class, uint8_t subclass, bool (*callback)(struct pci_config_header_common *)) {
    if(driver_callbacks[class][subclass] == 0)
    {
        driver_callbacks[class][subclass] = pmalloc(0x1000);
        *((uint64_t*)driver_callbacks[class][subclass]) = 1;
    }
    uint64_t * dvc_group = ((uint64_t*)driver_callbacks[class][subclass]);
    dvc_group[*dvc_group] = (uint64_t)callback;
    *dvc_group++;
}