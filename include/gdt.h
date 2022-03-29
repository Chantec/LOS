#ifndef INCLUDE_GDT_H_
#define INCLUDE_GDT_H_


#include "types.h"


//GDT global desciptor table
typedef
struct gdt_entry
{
    //一共8个字节
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;//包括 type s dpl p
    uint8_t granularity;//limit_high(4) avl l d/b G 
    uint8_t base_high;
}__attribute__((packed)) gdt_entry_t;

//GDTR 
typedef 
struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) gdt_ptr_t;

//初始化gdt
void init_gdt();

//初始化gdtr asm实现
extern void gdt_flush(uint32_t);


#endif 