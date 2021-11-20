#include "cons.h"

uint8_t default_buffer[DEFAULT_TERM_HEIGHT*DEFAULT_TERM_WIDTH];

struct terminal_handle default_term = {
    16, 16,
    DEFAULT_TERM_WIDTH, DEFAULT_TERM_HEIGHT,
    0xff000000,0xffffffff,
    1,
    default_buffer,
    0,
    0,
};

struct terminal_handle * current_term = &default_term;

void show()
{
    for(uint64_t j = 0; j < current_term->h; j++) {
        uint8_t line_buffer[current_term->w+1];
        for(uint64_t i = 0; (i < current_term->w) && (line_buffer[i]); i++) {
            line_buffer[i] = current_term->buffer[current_term->w * j + i];
        }
        line_buffer[current_term->w] = 0;
        putString(line_buffer,current_term->x,current_term->y + j*12*current_term->zoom,current_term->foreground,current_term->background,current_term->zoom);
    }
}

void print(uint8_t * str)
{
    for(uint64_t i = 0; str[i]; i++) {
        if(str[i] == '\n')
        {
            current_term->i = 0;
            current_term->j++;
            current_term->j%=current_term->h;
        } else {
            current_term->buffer[current_term->w*current_term->j+current_term->i] = str[i];
            current_term->i++;
            if(current_term->i == current_term->w) {
                current_term->i = 0;
                current_term->j++;
                current_term->j%=current_term->h;
            }
        }
    }
    show();
}

void print_num(uint64_t num)
{
    uint8_t numstr[19];
    itohex(num,numstr);
    numstr[18] = 0;
    print(numstr);
}

void switch_term(struct terminal_handle * handle)
{
    current_term = handle;
}