#include "mmap.h"

struct memory_block_seg segments[MAX_ALLOC];

void register_mmap(struct mmap_entry * base, uint64_t count)
{
    uint64_t k = 0;
    for(uint64_t i = 0; i < count; i++)
    {
        if(base[i].type == 1)
        {
            segments[k].base = base[i].base;
            segments[k].len = base[i].length;
            segments[k].allocated = false;
            k++;
        }
    }
}

void regroup()
{
    sort_merge(0,MAX_ALLOC >> 1, MAX_ALLOC, compare_base);
    uint64_t i = 0;

    while(i < MAX_ALLOC - 1) {
        uint64_t k = i + 1;
        while( (k < MAX_ALLOC) && // loop condition
        ((segments[i].base + segments[i].len)==segments[k].base) && // adjacency clause
        (segments[k].len != 0) && (segments[i].len != 0) && // validity clause
        !(segments[k].allocated) && !(segments[i].allocated)) // free clause
        {
            segments[i].len += segments[k].len;
            segments[k].base = 0;
            segments[k].len = 0;
            segments[k].allocated = 0;
            k++;
        }
        i = k;
    }
    sort();
}

bool compare_len(uint64_t i, uint64_t j)
{
    return segments[i].len < segments[j].len;
}

bool compare_base(uint64_t i, uint64_t j)
{
    return segments[i].base > segments[j].base;
}

void sort_merge(uint64_t l, uint64_t r, uint64_t sz, bool(* comp)(uint64_t,uint64_t))
{
    if (sz == 1) {
        return;
    }
    uint64_t nsz = sz >> 1;
    sort_merge(l, l + (nsz>>1), nsz, comp);
    sort_merge(r, r + (nsz>>1), nsz, comp);
    uint64_t lp = l;
    uint64_t rp = r;
    while(((lp-l) < sz) && ((rp-l) < sz))
    {
        if(comp(lp,rp)) {
            struct memory_block_seg s = segments[rp];
            for(uint64_t i = rp; i > lp; i--)
            {
                segments[i] = segments[i-1];
            }
            segments[lp] = s;
            lp ++;
            rp ++;
        } else {
            lp ++;
        }
    }
}

void sort()
{
    sort_merge(0,MAX_ALLOC >> 1, MAX_ALLOC, compare_len);
}

void * pmalloc(uint64_t size) // worst fit
{
    size = (size & 0xfff) ? (size & 0xfffffffffffff000) + 0x1000 : size; // page allocation
    size = (size) ? size : 0x1000;
    for(uint64_t i = 0; i < MAX_ALLOC; i++) {
        if ((!(segments[i].allocated)) && (size <= segments[i].len)) {
            if ((segments[MAX_ALLOC-1].len == 0) && (size < segments[i].len)) {
                segments[MAX_ALLOC-1].base = segments[i].base;
                segments[MAX_ALLOC-1].len = size;
                segments[MAX_ALLOC-1].allocated = true;
                segments[i].base += size;
                segments[i].len -= size;
                sort();
            } else {
                segments[i].allocated = true;
            }
            return (void *)(segments[i].base);
        }
    }
    return 0;
}
void free(void * ptr) 
{
    for(uint64_t i = 0; i < MAX_ALLOC; i++) {
        if (segments[i].base == (uint64_t)ptr) {
            segments[i].allocated = false;
        }
    }
}