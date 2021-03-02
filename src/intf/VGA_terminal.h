#pragma once

#include<stdint.h>
#include<stddef.h>

typedef enum VGA_COLOR 
{
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    YELLOW,
    WHITE
} VGA_COLOR;

uint8_t     VGA_color(VGA_COLOR fg, VGA_COLOR bg);
uint16_t    VGA_data(char chr, VGA_COLOR fg, VGA_COLOR bg);
void        VGA_clear(char chr, VGA_COLOR color);
void        VGA_print(char chr, VGA_COLOR fg, VGA_COLOR bg);
void        VGA_println(char* string, VGA_COLOR fg, VGA_COLOR bg);
void        VGA_newline();
