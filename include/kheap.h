#ifndef INCLUDE_KHEAP1_H_
#define INCLUDE_KHEAP1_H_
//liangtodo above INCLUDE_KHEAP_H_ jiu jin bu lai le 

#include "types.h"

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x10000

//分配函数 a: aligned p : physical
//这个1是指的刚开始没有建立heap的时候 直接调用从内存后面要的空间
uint32_t kmalloc_a1(uint32_t size);
uint32_t kmalloc_p1(uint32_t size,uint32_t *phys);
uint32_t kmalloc_ap1(uint32_t size,uint32_t *phys);
uint32_t kmalloc1(uint32_t size);//vanilla 普通的 

extern uint32_t kern_end;
uint32_t placement_address;//liangtodo in this use = &kern_end does not work proprely


//heap
typedef 
struct header{
    struct header *prev;
    struct header *next;
    uint32_t allocated:1;
    uint32_t length:31;
}header_t;

void init_heap();

void *kmalloc(uint32_t len);

void kfree(void *p);
#endif