#include "idt.h"

void ignore_int()
{
    VGA_println("Interrupt\n", WHITE, BLACK);
}

void setup_idt(IDTDesc_ptr * desc_ptr)
{
    IDTDescr * idtr = (IDTDescr *)desc_ptr->ptr;

    for(size_t r = 0; r < desc_ptr->size; r++)
    {
        IDTDescr idt = idtr[r];

        
    }
}