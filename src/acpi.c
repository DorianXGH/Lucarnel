#include "includes/acpi.h"

bool do_checksum(struct ACPISDTHeader* table_header)
{
    uint8_t sum = 0;
    for (int i = 0; i < table_header->length; i++)
    {
        sum += ((uint8_t*)table_header)[i];
    }
    return sum == 0;
}