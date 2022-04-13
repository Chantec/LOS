#include "kheap.h"
#include "paging.h"


//placement_address就是内核和分配页表已经使用过的地址的最后
//int internal 内部
uint32_t kmalloc_int(uint32_t sz,int align,uint32_t *phys)
{
    if(align)
    {
         //先做个对其 向下取整 再加1
         //bug002 这里直接做了向下取整并且+1024 没有考虑本来就已经对齐的情况
         if(placement_address&0x00000FFF)
         {
            placement_address&=0xFFFFF000;//一个页是4k 12bit 
            placement_address+=0x1000;
         }
        
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


//ch 7 add

//找到最小的满足需求的hole
static int32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap)
{
   for(int i=0;i<heap->index.size;++i)
   {
      header_t *header = (header_t *)lookup_ordered_array(i, &heap->index);
      if(header->size>=size)//liangtodo 这里难道不用减去header和footer吗
      {
         return i;
      }
   }
   return -1;//找不到
}

static void expand(uint32_t new_size,heap_t *heap)
{
   //对齐
   if(new_size&0xFFF)
   {
      new_size&=0xFFFFF000;
      new_size+=0x1000;
   }

   uint32_t old_size=heap->end_addr-heap->start_addr;
   for(uint32_t i =old_size;i<new_size;i+=0x1000)
   {
      alloc_frame(get_page(heap->start_addr+i,1,kernel_directory),heap->supervisor,heap->readonly?0:1);
   }
   heap->end_addr=heap->start_addr+new_size;
}


//返回新的大小 
static uint32_t contract (uint32_t new_size,heap_t *heap)
{
   //check 
   //对齐
   if(new_size&0xFFF)
   {
      new_size&=0xFFFFF000;
      new_size+=0x1000;
   }
   
   uint32_t old_size=heap->end_addr-heap->start_addr;
   for(uint32_t i =old_size;i>new_size;i-=0x1000)
   {
      free_frame(get_page(heap->start_addr+i,0,kernel_directory));
   }
   heap->end_addr=heap->start_addr+new_size;
   return new_size;
}

void *alloc(uint32_t size,uint8_t page_align,heap_t *heap)
{
   uint32_t new_size=size+sizeof(header_t)+sizeof(footer_t);

   int32_t i=find_smallest_hole(new_size,page_align,heap);

   if(i==-1)
   {
      //找不到
   }
   
}