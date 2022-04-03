#include "mmap.h"

struct memory_block_seg segments[MAX_ALLOC];


void register_mmap(struct mmap_entry * base, uint64_t count)
{
    uint64_t k = 0;
    for(uint64_t i = 0; i < count && k<MAX_ALLOC; i++)
    {
        if(base[i].type == 1)
        {
            segments[k].base = base[i].base;
            segments[k].len = base[i].length;
            segments[k].allocated = false;
            k++;
        }
    }
    sort();
}

void regroup()
{
    sort_insert(0,MAX_ALLOC >> 1, MAX_ALLOC, compare_base);
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

bool compare_len(struct memory_block_seg * i, struct memory_block_seg * j)
{
    return i->len < j->len;
}

bool compare_base(struct memory_block_seg * i, struct memory_block_seg * j)
{
    return i->base < j->base;
}


void sort_insert(uint64_t l, uint64_t r, uint64_t sz, bool(* comp)(struct memory_block_seg *,struct memory_block_seg *))
{
    for (uint64_t i = 1; i < sz; i++)
    {
        struct memory_block_seg key = segments[i];
        int64_t j;
        for (j = i - 1; j >= 0 && comp(&(segments[j]),&key); j--)
        {
            segments[j + 1] = segments[j];
        }
        segments[j + 1] = key;
    }
}

void sort()
{
    sort_insert(0,MAX_ALLOC >> 1, MAX_ALLOC, compare_len);
}

void * bitmap_allocator(uint64_t sec) {
    uint64_t sec_size = segments[sec].len;
    uint64_t bitmap_size_pg = ((sec_size & 0xfff) == 0) ? sec_size >> 12 : (sec_size >> 12) + 1; // nombre de pages dans la section
    uint64_t bitmap_size_abs = ((bitmap_size_pg & 0x7) == 0) ? bitmap_size_pg >> 3 : (bitmap_size_pg >> 3) + 1; // taille de la bitmap en octets
    uint64_t bitmap_size = (bitmap_size_abs & 0xfff) ? (bitmap_size_abs & 0xfffffffffffff000) + 0x1000 : bitmap_size_abs; // taille de la bitmap en pages
    /*print("\nbtm sz ");
    print_num(bitmap_size_pg);
    print(" pg ");
    print_num(bitmap_size);
    print("\nbtm ");*/
    uint64_t allocated = segments[sec].first_free;
    /*print_num(allocated);
    print(" ");
    print_num(segments[sec].base);
    print("  ");*/
    uint64_t alloc_b_idx = (allocated-bitmap_size-(segments[sec].base)) >> 12; // bit index
    /*print_num(alloc_b_idx);
    print(" ");*/
    uint64_t alloc_qw_idx = alloc_b_idx >> 6; // 64bit uint index
    /*print_num(alloc_qw_idx);
    print(" ");*/
    uint64_t * bitmap = (uint64_t *) segments[sec].base;
    bitmap[alloc_qw_idx] |= 1 << (alloc_b_idx & 0x3f); // mark page as allocated

    uint64_t nxt = alloc_qw_idx;
    //print("\n find new free ");
    while((~(bitmap[nxt]) == 0)&&(nxt < (bitmap_size_abs >> 3))) // try to find unallocated pages
    {
        
        /*print_num(bitmap[nxt]);
        print(" ");*/
        nxt++;
    }
    uint64_t k = __builtin_ffsll(~(bitmap[nxt])); // first bit to free page
    uint64_t free_p_num = ((nxt << 6) + k - 1); // page numberé
    /*print("\n res ");
    print_num(k);
    print(" ");
    print_num(free_p_num);
    print(" ");
    print_num(bitmap_size_pg - (bitmap_size >> 12));
    print("\n");*/
    if (k == 0 || (free_p_num >= bitmap_size_pg - (bitmap_size >> 12))) // if the section is full
    {
        segments[sec].full = 1; // mark it as such
        //print("\n  set full  \n");
    }
    else
    {
        segments[sec].first_free = bitmap_size + segments[sec].base + (free_p_num << 12);
        /*print("\n  next  ");
        print_num(segments[sec].first_free);
        print("\n");*/
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
    uint64_t sec_size = segments[sec].len;
    uint64_t bitmap_size_pg = ((sec_size & 0xfff) == 0) ? sec_size >> 12 : (sec_size >> 12) + 1; // nombre de pages dans la section
    uint64_t bitmap_size_abs = ((bitmap_size_pg & 0x7) == 0) ? bitmap_size_pg >> 3 : (bitmap_size_pg >> 3) + 1; // taille de la bitmap en octets
    uint64_t bitmap_size = (bitmap_size_abs & 0xfff) ? (bitmap_size_abs & 0xfffffffffffff000) + 0x1000 : bitmap_size_abs; // taille de la bitmap en pages x taille de page

    if(segments[sec].len >= 0x2000)
    {
        segments[sec].allocated = 1;
        segments[sec].allocated_count = 0;
        segments[sec].bitmap_allocated = 1;
        segments[sec].full = 0;
        segments[sec].first_free = segments[sec].base + bitmap_size;
        // print("init bitmap ");
        init_bitmap_allocator(sec);
        // print("done\n");
    }
}

uint64_t find_new_bitmap_allocator()
{
    for(uint64_t i = MAX_ALLOC-1; i >= 0; i--) {
        if((segments[i].len != 0) && !(segments[i].allocated)) {
            /*print("converting ");
            print_num(i);
            print("\n");*/
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

void * pmalloc_constraint(uint64_t size, uint64_t max_add)
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
            if ((!(segments[i].allocated)) && (size <= segments[i].len) && ((segments[i].base + size - 1) <= max_add)) {
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

void * pmalloc(uint64_t size) // worst fit
{
    pmalloc_constraint(size,-1);
}

void bitmap_free(uint64_t sec, void * ptr)
{
    uint64_t sec_size = segments[sec].len;
    uint64_t bitmap_size_pg = ((sec_size & 0xfff) == 0) ? sec_size >> 12 : (sec_size >> 12) + 1; // nombre de pages dans la section
    uint64_t bitmap_size_abs = ((bitmap_size_pg & 0x7) == 0) ? bitmap_size_pg >> 3 : (bitmap_size_pg >> 3) + 1; // taille de la bitmap en octets
    uint64_t bitmap_size = (bitmap_size_abs & 0xfff) ? (bitmap_size_abs & 0xfffffffffffff000) + 0x1000 : bitmap_size_abs; // taille de la bitmap en pages x taille de page
    uint64_t page = ((uint64_t)ptr-segments[sec].base-bitmap_size)>>12;
    
    uint64_t page_qw_idx = page >> 6; // 64bit uint index
    uint64_t * bitmap = (uint64_t *) segments[sec].base;
    bitmap[page_qw_idx] &= ~((uint64_t)(1 << (page & 0x3f))); // set to 0
    if(segments[sec].full){
        segments[sec].full = 0;
        segments[sec].first_free = (((uint64_t) ptr) >> 12) << 12;
    }
    if ((uint64_t) ptr < segments[sec].first_free)
    {
        segments[sec].first_free = (((uint64_t) ptr) >> 12) << 12;
    }
}

void free(void * ptr) 
{
    for(uint64_t i = 0; i < MAX_ALLOC; i++) {
        if (segments[i].base == (uint64_t)ptr) {
            segments[i].allocated = false;
        }
        if (segments[i].base <= (uint64_t)ptr && (uint64_t)ptr < segments[i].base+segments[i].len && segments[i].bitmap_allocated) {
            bitmap_free(i,ptr);
        }
    }
}

void show_map() {
    print("\n");
    for(uint64_t i = 0; i < MAX_ALLOC; i++) {
        if (segments[i].len != 0) {
            if(i%10 == 0)
                print("\n");
            print("base ");
            print_num(segments[i].base);
            print(" len ");
            print_num(segments[i].len);
            if(segments[i].allocated)
            {
                print(" allocated");
            }
            if(segments[i].bitmap_allocated)
            {
                print(" bitmap");
            }
            if(segments[i].full)
            {
                print(" full");
            }
            print("\n");
            
        }
    }
    print("\n");
}