#include "includes/utils.h"

void itohex(uint64_t num, uint8_t buffer[19])
{
    uint8_t digits[] = "0123456789abcdef";
    buffer[0] = '0';
    buffer[1] = 'x';
    for(int i = 0; i < 16; i++)
    {
        buffer[17-i] = digits[(num>>(4*i))&0xF];
    }
    buffer[18] = 0;
}

bool arrcmp(uint8_t* arr1, uint8_t* arr2, uint8_t len)
{
    bool eq = true;
    for(int i = 0; i < len; i++)
    {
        eq &= (arr1[i] == arr2[i]);
    }
    return eq;
}