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

void * bitmap_allocator(uint64_t sec) {
    uint64_t sec_size = segments[sec].len;
    uint64_t bitmap_size_pg = ((sec_size & 0xfff) == 0) ? sec_size >> 12 : (sec_size >> 12) + 1; // nombre de pages dans la section
    uint64_t bitmap_size_abs = ((bitmap_size_pg & 0x7) == 0) ? bitmap_size_pg >> 3 : (bitmap_size_pg >> 3) + 1; // taille de la bitmap en octets
    uint64_t bitmap_size = (bitmap_size_abs & 0xfff) ? (bitmap_size_abs & 0xfffffffffffff000) + 0x1000 : bitmap_size_abs; // taille de la bitmap en pages

    uint64_t allocated = segments[sec].first_free;
    uint64_t alloc_b_idx = (allocated-bitmap_size) >> 12;
    uint64_t alloc_qw_idx = alloc_b_idx >> 6;
    uint64_t * bitmap = (uint64_t *) segments[sec].base;
    bitmap[alloc_qw_idx] |= 1 << (alloc_b_idx & 0x3f);

    uint64_t nxt = alloc_qw_idx;
    while((~(bitmap[nxt]) == 0)&&(nxt < (bitmap_size_abs >> 3)))
    {
        nxt++;
    }
    uint64_t k = __builtin_ffsll(~(bitmap[nxt]));
    uint64_t free_p_num = ((nxt << 6) + k - 1);
    if (k == 0 || (free_p_num >= bitmap_size_pg)) // todo fix unaligned
    {
        segments[sec].full = 1;
    }
    else
    {
        segments[sec].first_free = free_p_num << 12;
    }
    segments[sec].allocated_count++;
    return (void *) allocated;
}

void init_bitmap_allocator(uint64_t sec)
{
    uint64_t sec_size = segments[sec].len;
    uint64_t bitmap_size_pg = ((sec_size & 0xfff) == 0) ? sec_size >> 12 : (sec_size >> 12) + 1; // nombre de pages dans la section
    uint64_t bitmap_size_abs = ((bitmap_size_pg & 0x7) == 0) ? bitmap_size_pg >> 3 : (bitmap_size_pg >> 3) + 1; // taille de la bitmap en octets
    uint8_t * bitmap = (uint8_t *) segments[sec].base;
    for (uint64_t i = 0; i < bitmap_size_abs; i++)
        bitmap[i] = 0;
} 

void convert_to_bitmap_allocator(uint64_t sec)
{
    if(segments[sec].len > 0x2000)
    {
        segments[sec].allocated = 1;
        segments[sec].allocated_count = 0;
        segments[sec].bitmap_allocated = 1;
        segments[sec].full = 0;
        init_bitmap_allocator(sec);
    }
}

uint64_t find_new_bitmap_allocator()
{
    for(uint64_t i = MAX_ALLOC-1; i >= 0; i--) {
        if((segments[i].len != 0) && !(segments[i].allocated)) {
            convert_to_bitmap_allocator(i);
            return i+1;
        }
    }
    return 0;
}

void * bitmap_allocate()
{
    for(uint64_t i = 0; i < MAX_ALLOC; i++) {
        if (segments[i].bitmap_allocated && !(segments[i].full)) {
            return bitmap_allocator(i);
        }
    }
    return 0;
}

void * pmalloc(uint64_t size) // worst fit
{
    size = (size & 0xfff) ? (size & 0xfffffffffffff000) + 0x1000 : size; // page allocation
    size = (size) ? size : 0x1000;
    if(size == 0x1000) // use single page allocator (ie : bitmap)
    {
        void * addr = bitmap_allocate();
        if (addr)
        {
            return addr;
        } else {
            uint64_t sec = find_new_bitmap_allocator();
            if (sec) {
                return bitmap_allocator(sec-1);
            }
        }
    } else {  // use range allocator
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