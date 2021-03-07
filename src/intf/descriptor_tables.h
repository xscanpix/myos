#pragma once

#include "common.h"

#pragma pack (8)
struct gdt_entry_struct 
{
    uint16_t    limit_low;
    uint16_t    base_low;
    uint8_t     base_middle;
    uint8_t     access;
    uint8_t     flags;
    uint8_t     base_high;
};
typedef struct gdt_entry_struct gdt_entry_t;

#pragma pack (8)
struct gdt_ptr_struct
{
    uint16_t    limit;
    uint32_t*    base;
};
typedef struct gdt_ptr_struct gdt_ptr_t;

#pragma pack (8)
struct idt_entry_struct
{
    uint16_t    base_low;
    uint16_t    selector;
    uint8_t     zero;
    uint8_t     flags;
    uint16_t    base_high;
};
typedef struct idt_entry_struct idt_entry_t;

#pragma pack (8)
struct idt_ptr_struct
{
    uint16_t    limit;
    uint32_t*    base;
};
typedef struct idt_ptr_struct idt_ptr_t;

void init_descriptor_tables();