#include "includes/pci.h"
#include "video/video.h"
#include "video/cons.h"

void init_pci_tree(struct MCFG* mcfg) 
{
    uint64_t num_entries = (mcfg->header.length - sizeof(mcfg->header) - sizeof(mcfg->reserved))/sizeof(struct MCFGEntry);
    for(uint64_t i = 0; i < num_entries; i++) {
        print("\npci segment group \n");
        uint8_t basestr[19];
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
        
    }
}

struct pci_config_header_common * get_PCI_device_cfg_space(uint64_t PCI_config_base, uint8_t bus, uint8_t device, uint8_t function)
{
    return (struct pci_config_header_common *)(PCI_config_base + (bus << 20 | device << 15 | function << 12));
}