#include "VGA_terminal.h"

#include <stdarg.h>

static const size_t COLUMNS = 80;
static const size_t ROWS    = 25;

static uint16_t * const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t column = 0;
static size_t row    = 0;

uint8_t     VGA_color(VGA_COLOR fg, VGA_COLOR bg)
{
    return (uint8_t) fg | (uint8_t) bg << 4;
}

uint16_t    VGA_data(const char chr, VGA_COLOR fg, VGA_COLOR bg)
{
    return (uint16_t) chr | (uint16_t) VGA_color(fg, bg) << 8;
}

void        VGA_clear(const char chr, VGA_COLOR color)
{
    uint16_t * buffer = VGA_MEMORY;

    for(size_t col = 0; col < COLUMNS; col++)
    {
        for(size_t row = 0; row < ROWS; row++)
        {
            buffer[col + COLUMNS * row] = VGA_data(chr, WHITE, color);
        }
    }
}

void        VGA_print(const char chr, VGA_COLOR fg, VGA_COLOR bg)
{
    switch(chr)
    {
        case '\n':
        {
            VGA_newline();
        } break;
        default:
        {
            uint16_t * buffer = VGA_MEMORY;

            buffer[row * COLUMNS + column] = VGA_data(chr, fg, bg);

            column++;

            if(column >= COLUMNS)
            {
                VGA_newline();
            }
        }break;
    }
}

void        VGA_println(const char * string, VGA_COLOR fg, VGA_COLOR bg)
{
    size_t count = 0;

    char c;

    while((c = string[count++]) != '\0')
    {
        VGA_print(c, fg, bg);
    }
}

void        VGA_newline()
{
    uint16_t * buffer = VGA_MEMORY;

    if(column < COLUMNS)
    {
        for(size_t i = row * COLUMNS + column; i < (row + 1) * COLUMNS; i++)
        {
            buffer[i] = VGA_data(' ', DEFAULT_BG_COLOR, DEFAULT_FG_COLOR);
        }
    }
    row = (row + 1) % ROWS;
    column = 0;

    if(row == 0)
    {
        for(size_t i = COLUMNS; i < COLUMNS * ROWS; i++)
        {
            buffer[i - COLUMNS] = buffer[i];
        }
        for(size_t i = (ROWS - 1) * COLUMNS; i < ROWS * COLUMNS; i++)
        {
            buffer[i] = VGA_data(' ', DEFAULT_BG_COLOR, DEFAULT_FG_COLOR);
        }
    }
}

void UlongToString(unsigned long number, char * str)
{
    // TODO: Should remove leading zeros?
    static const unsigned long POWERS[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

    for(int c = 9; c >= 0; c--)
    {
        char digit = '0';
        unsigned long power = POWERS[c];

        while(number >= power)
        {
            number -= power;
            digit++;
        }

        str[9-c] = digit;
    }
}

void        VGA_printf(const char * format, ...)
{
    va_list argp;
    va_start(argp, format);

    while(*format != '\0')
    {
        if(*format == '%')
        {
            format++;

            if(*format == '%')
            {
                VGA_print('%', DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
            } 
            else if(*format == 'c') 
            {
                char c = va_arg(argp, int);
                VGA_print(c, DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
            }
            else if(*format == 'd' || *format == 'i')
            {
                int i = va_arg(argp, int);

                if(i < 0)
                {
                    VGA_print('-', DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
                    i = -i;
                }

                char buf[32] = {'\0'};

                UlongToString(i, buf);

                VGA_println(buf, DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
            }
            else
            {
                VGA_println("Format: ", DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
                VGA_print(*format, DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
                VGA_println(" not supported.", DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
            }
        } else 
        {
            VGA_print(*format, DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
        }

        format++;
    }

    va_end(argp);
}