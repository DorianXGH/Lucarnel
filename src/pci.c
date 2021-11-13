#include "includes/pci.h"
#include "video/video.h"

extern struct stivale_struct stivale_global_info;

void init_pci_tree(struct MCFG* mcfg) 
{
    uint64_t num_entries = (mcfg->header.length - sizeof(mcfg->header) - sizeof(mcfg->reserved))/sizeof(struct MCFGEntry);
    for(uint64_t i = 0; i < num_entries; i++) {
        putString("pci\0",64,64+128*i,&stivale_global_info,0x00FF00FF,0xFF000000,2);
        uint8_t basestr[19];
        itohex(mcfg->entries[i].base,basestr);
        putString(basestr,64,64+128*i+16,&stivale_global_info,0x00FF00FF,0xFF000000,2);
        itohex(mcfg->entries[i].segment_group,basestr);
        putString(basestr,64,64+128*i+32,&stivale_global_info,0x00FF00FF,0xFF000000,2);
        itohex(mcfg->entries[i].bus_start,basestr);
        putString(basestr,64,64+128*i+48,&stivale_global_info,0x00FF00FF,0xFF000000,2);
        itohex(mcfg->entries[i].bus_end,basestr);
        putString(basestr,64,64+128*i+64,&stivale_global_info,0x00FF00FF,0xFF000000,2);
        
    }
}