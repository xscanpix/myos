#include "VGA_terminal.h"

static const size_t COLUMNS = 80;
static const size_t ROWS    = 25;

static uint16_t * const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t column = 0;
static size_t row    = 0;

uint8_t     VGA_color(VGA_COLOR fg, VGA_COLOR bg)
{
    return (uint8_t) fg | (uint8_t) bg << 4;
}

uint16_t    VGA_data(char chr, VGA_COLOR fg, VGA_COLOR bg)
{
    return (uint16_t) chr | (uint16_t) VGA_color(fg, bg) << 8;
}

void        VGA_clear(char chr, VGA_COLOR color)
{
    uint16_t * buffer = VGA_MEMORY;

    for(size_t col = 0; col < COLUMNS; col++)
    {
        for(size_t row = 0; row < ROWS; row++)
        {
            buffer[col + COLUMNS * row] = VGA_data(chr, BLACK, color);
        }
    }
}

void        VGA_print(char chr, VGA_COLOR fg, VGA_COLOR bg)
{
    if(chr == '\n')
    {
        VGA_newline();
    }

    uint16_t * buffer = VGA_MEMORY;

    if(column < (COLUMNS - 1))
    {
        column++;
    } 
    
    if(row >= (ROWS - 1)) 
    {
        row = 0;
    }

    buffer[column + COLUMNS * row] = VGA_data(chr, fg, bg);
}

void        VGA_println(char* string, VGA_COLOR fg, VGA_COLOR bg)
{
    
}

void        VGA_newline()
{
    row = (row + 1) % ROWS;
}