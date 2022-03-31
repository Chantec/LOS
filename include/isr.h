#ifndef INCLUDE_ISR_H_
#define INCLUDE_ISR_H_

#include "types.h"

typedef 
struct registers
{
    uint32_t ds;
    uint32_t edi,esi,ebp,esp,ebx,edx,ecx,eax;
    uint32_t int_no,err_code;
    uint32_t eip,cs,eflags,useresp,ss;//这个应该是cli的时候自动
}registers_t;

typedef void (*isr_t)(registers_t);//函数指针

#define IRQ0 32 


void register_interrupt_handler(uint8_t n,isr_t handler);//注册中断处理程序

#endif
