#ifndef INCLUDE_KHEAP_H_
#define INCLUDE_KHEAP_H_


#include "types.h"
#include "ordered_array.h"


#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000
#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000



//分配函数 a: aligned p : physical

uint32_t kmalloc_a(uint32_t size);
uint32_t kmalloc_p(uint32_t size,uint32_t *phys);
uint32_t kmalloc_ap(uint32_t size,uint32_t *phys);
uint32_t kmalloc(uint32_t size);//vanilla 普通的 

extern uint32_t kern_end;
uint32_t placement_address;//liangtodo in this use = &kern_end does not work proprely


typedef struct 
{
    uint32_t magic;
    uint8_t is_hole;//这里可以优化一下啊
    uint32_t size;//all block 包括header 和 footer
}header_t;

typedef struct 
{
    uint32_t magic;
    header_t *header;
}footer_t;

typedef struct 
{
    ordered_array_t index;//
    uint32_t start_addr;
    uint32_t end_addr;
    uint32_t max_addr;
    uint8_t supervisor;//liangtodo
    uint8_t readonly;//liangtodo
}heap_t;

heap_t * create_heap(uint32_t start,uint32_t end,uint32_t max,uint8_t supervisor,uint8_t readonly);

void *alloc(uint32_t size,uint8_t page_align,heap_t *heap);

void free(void *p,heap_t *heap);

#endif