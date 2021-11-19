#include "cons.h"

uint8_t default_buffer[DEFAULT_TERM_HEIGHT*DEFAULT_TERM_WIDTH];

struct terminal_handle default_term = {
    64, 0,
    DEFAULT_TERM_HEIGHT, DEFAULT_TERM_WIDTH,
    0xffffffff,0xff000000,
    1,
    default_buffer,
    0,
    0,
};

struct terminal_handle * currect_term = &default_term;

void show()
{
    
}

void print(uint8_t * str)
{
    for(uint8_t * cur = str; !(*cur); cur++) {
        if(*cur == '\n')
        {
            currect_term->i = 0;
            currect_term->j++;
            currect_term->j%=currect_term->h;
        } else {
            currect_term->buffer[currect_term->w*currect_term->j+currect_term->i] = *cur;
            currect_term->i++;
            if(currect_term->i == currect_term->w) {
                currect_term->i = 0;
                currect_term->j++;
                currect_term->j%=currect_term->h;
            }
        }
    }

}

void print_num(uint64_t num)
{

}

void switch_term(struct terminal_handle * handle)
{
    currect_term = handle;
}