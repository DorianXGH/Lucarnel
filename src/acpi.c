#include "includes/acpi.h"

bool do_checksum(struct ACPISDTHeader* table_header)
{
    unsigned char sum = 0;
 
    for (int i = 0; i < table_header->length; i++)
    {
        sum += ((char *) table_header)[i];
    }
 
    return sum == 0;
}