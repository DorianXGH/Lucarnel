#include "alloc.h"

void * kmalloc(uint64_t size)
{
    return pmalloc(size);
}
void * kfree(void * ptr, uint64_t size)
{
    free(ptr);
    return ptr;
}

void * kpmalloc(uint64_t size)
{
    return pmalloc(size);
}

void * kpfree(void * ptr, uint64_t size)
{
    free(ptr);
    return ptr;
}