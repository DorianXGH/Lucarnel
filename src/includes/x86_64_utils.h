#pragma once
#include "memory_structs.h"

void _lgdt(struct GDTD *);
void _lcr3(struct CR3 *);