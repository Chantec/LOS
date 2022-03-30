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

#endif
