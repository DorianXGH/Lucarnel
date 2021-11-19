#include "includes/pci.h"
#include "video/video.h"

void init_pci_tree(struct MCFG* mcfg) 
{
    uint64_t num_entries = (mcfg->header.length - sizeof(mcfg->header) - sizeof(mcfg->reserved))/sizeof(struct MCFGEntry);
    for(uint64_t i = 0; i < num_entries; i++) {
        putString("pci\0",64,64+128*i,0x00FF00FF,0xFF000000,1);
        uint8_t basestr[19];
        itohex(mcfg->entries[i].base,basestr);
        putString(basestr,64,64+128*i+16,0x00FF00FF,0xFF000000,1);
        itohex(mcfg->entries[i].segment_group,basestr);
        putString(basestr,64,64+128*i+32,0x00FF00FF,0xFF000000,1);
        itohex(mcfg->entries[i].bus_start,basestr);
        putString(basestr,64,64+128*i+48,0x00FF00FF,0xFF000000,1);
        itohex(mcfg->entries[i].bus_end,basestr);
        putString(basestr,64,64+128*i+64,0x00FF00FF,0xFF000000,1);
        
    }
}

struct pci_config_header_common * get_PCI_device_cfg_space(uint64_t PCI_config_base, uint8_t bus, uint8_t device, uint8_t function)
{
    return (struct pci_config_header_common *)(PCI_config_base + (bus << 20 | device << 15 | function << 12));
}