#ifndef INCLUDE_KHEAP_H_
#define INCLUDE_KHEAP_H_


#include "types.h"

//分配函数 a: aligned p : physical

uint32_t kmalloc_a(uint32_t size);

uint32_t kmalloc_p(uint32_t size,uint32_t *phys);

uint32_t kmalloc_ap(uint32_t size,uint32_t *phys);

uint32_t kmalloc(uint32_t size);//vanilla 普通的 

extern uint32_t kern_end;

uint32_t placement_address;//liangtodo in this use = &kern_end does not work proprely


#endif