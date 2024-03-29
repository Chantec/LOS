#ifndef INCLUDE_IDT_H_
#define INCLUDE_IDT_H_

#include "types.h"

//IDT interrupt desciptor table
typedef
struct idt_entry
{
   uint16_t base_low;//中断处理程序的入口地址
   uint16_t sel;//段选择子
   uint8_t always0;
   uint8_t flags;
   uint16_t base_high;
}__attribute__((packed)) idt_entry_t;

//IDTR 
typedef 
struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) idt_ptr_t;

//中断处理程序 intel 使用的
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
//isr结束

//irq
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
//irq结束


//初始化idt
void init_idt();

//初始化idtr asm实现
extern void idt_flush(uint32_t);

#endif 