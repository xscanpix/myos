#include "VGA_terminal.h"

void kernel_main(void)
{
    VGA_clear(' ', BLACK);
    VGA_println("Hello World!\n", BLACK, WHITE);
    VGA_println("Hello World!\n", BLACK, WHITE);
}