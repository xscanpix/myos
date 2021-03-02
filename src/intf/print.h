#pragma once

#include<stdint.h>
#include<stddef.h>

void init_vga_buffer(void);
void print_clear(void);
void print_chr(const uint8_t character);
void print_cstr(const uint8_t const * string);
void print_newline();
