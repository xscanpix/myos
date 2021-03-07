#include "descriptor_tables.h"

//extern void gdt_flush(uint32_t*);

static void init_gdt();
static void gdt_set_gate(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

static gdt_entry_t gdt_entries[3];
static gdt_ptr_t gdt_ptr;

void init_descriptor_tables()
{
    init_gdt();
}

static void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_ptr_t) * 3) - 1;
    gdt_ptr.base  = (uint32_t*)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, (uint8_t)0b10011010, (uint8_t)0b11101111); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, (uint8_t)0b10001010, (uint8_t)0b11001111); // Data segment

    //gdt_flush((uint32_t*)&gdt_ptr);
}

static void gdt_set_gate(uint32_t selector, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    gdt_entries[selector].base_low      = base & 0xFFFF;
    gdt_entries[selector].base_middle   = (base >> 16) & 0xFF;
    gdt_entries[selector].base_high     = (base >> 24) & 0xFF;

    gdt_entries[selector].limit_low     = limit & 0xFFFF;
    
    gdt_entries[selector].flags         = (limit >> 16) & 0x0F;
    gdt_entries[selector].flags        |= flags & 0xF0;
    
    gdt_entries[selector].access        = access;
}