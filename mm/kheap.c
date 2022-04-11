#include "kheap.h"

//placement_address就是内核和分配页表已经使用过的地址的最后
uint32_t kmalloc_int(uint32_t sz,int align,uint32_t *phys)
{
    if(align)
    {
         //先做个对其 向下取整 再加1
        placement_address&=0xFFFFF000;//一个页是4k 12bit 
        placement_address+=0x1000;
    }
    if(phys)
    {
        *phys=placement_address;
    }
    uint32_t tmp=placement_address;
    placement_address+=sz;
    return tmp;
} 

uint32_t kmalloc_a(uint32_t size)
{
   return kmalloc_int(size,1,0);
}
uint32_t kmalloc_p(uint32_t size,uint32_t *phys)
{
   return kmalloc_int(size,0,phys);
}
uint32_t kmalloc_ap(uint32_t size,uint32_t *phys)
{
   return kmalloc_int(size,1,phys);
}
uint32_t kmalloc(uint32_t size)//vanilla 普通的 
{
   return kmalloc_int(size,0,0);
}