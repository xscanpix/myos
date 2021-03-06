#pragma once

#include "VGA_terminal.h"

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint16_t size;
    uint64_t ptr;
} IDTDesc_ptr;

typedef struct {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t type_attr; // type and attributes
   uint16_t offset_2; // offset bits 16..31
   uint32_t offset_3; // offset bits 32..63
   uint32_t zero;     // reserved
} IDTDescr;

void ignore_int();
void setup_idt(IDTDesc_ptr * desc_ptr);