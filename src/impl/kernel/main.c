#include "VGA_terminal.h"

#include "descriptor_tables.h"

void kernel_main(void)
{
    //init_descriptor_tables();

    VGA_clear(' ', BLACK);
    VGA_println("Hello World!\n", BLACK, WHITE);
    VGA_println("Hello World!\n", BLACK, WHITE);

    VGA_printf("Hello world, %%, c: %c, d/i: %d hello\nhellohello\nhello", '#', 1230);
}