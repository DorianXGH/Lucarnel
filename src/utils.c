#include "includes/utils.h"

void itohex(uint64_t num, uint8_t buffer[19])
{
    uint8_t digits[] = "0123456789abcdef";
    buffer[0] = '0';
    buffer[1] = 'x';
    for(int i = 0; i < 16; i++)
    {
        buffer[18-i] = digits[(num>>(4*i))&0xF];
    }
    buffer[19] = 0;
}