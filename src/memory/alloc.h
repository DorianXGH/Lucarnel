#pragma once
#include <stdint.h>
#include "mmap.h"

void * kmalloc(uint64_t size);
void * kfree(void * ptr, uint64_t size);

void * kpmalloc(uint64_t size);
void * kpfree(void * ptr, uint64_t size);