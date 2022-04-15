#include "kheap.h"
#include "paging.h"
#include "printk.h"


//placement_address就是内核和分配页表已经使用过的地址的最后
//int internal 内部
uint32_t kmalloc_int1(uint32_t sz,int align,uint32_t *phys)
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

uint32_t kmalloc_a1(uint32_t size)
{
   return kmalloc_int1(size,1,0);
}
uint32_t kmalloc_p1(uint32_t size,uint32_t *phys)
{
   return kmalloc_int1(size,0,phys);
}
uint32_t kmalloc_ap1(uint32_t size,uint32_t *phys)
{
   return kmalloc_int1(size,1,phys);
}
uint32_t kmalloc1(uint32_t size)//vanilla 普通的 
{
   return kmalloc_int1(size,0,0);
}


//ch 7 add


static void alloc_chunk(uint32_t start,uint32_t len);

static void free_chunk(header_t *chunk);

static void split_chunk(header_t *chunk,uint32_t len);

static void glue_chunk(header_t *chunk);

static uint32_t heap_max;

static header_t *heap_first;

void init_heap()
{
   heap_first=(header_t *)KHEAP_START;
   heap_max=(uint32_t)KHEAP_START+KHEAP_INITIAL_SIZE;

   header_t *cur_header=heap_first;
   cur_header->allocated=0;
   cur_header->next=0;
   cur_header->prev=0;
   cur_header->length=(uint32_t)KHEAP_INITIAL_SIZE;
}

void *kmalloc(uint32_t len)
{
   len+=sizeof(header_t);

   header_t *cur_header=heap_first;
   header_t *prev_header=0;
   
   while(cur_header)
   {
      if(cur_header->allocated==0&&cur_header->length>=len)
      {
         //printk("cur_header cur->all %d %d header:%8x\n",cur_header->allocated,cur_header->length,cur_header);
         split_chunk(cur_header,len);
         cur_header->allocated=1;
         //普通的情况 有空余的空间可用
         return (void *)((uint32_t)cur_header+(uint32_t)sizeof(header_t));
      }
      prev_header=cur_header;
      cur_header=cur_header->next;
   }
   //没有合适的空间 heap要扩张
   uint32_t chunk_start=(uint32_t)prev_header+prev_header->length;//最后的位置
   alloc_chunk(chunk_start,len);
   cur_header=(header_t *)chunk_start;
   cur_header->allocated=1;
   cur_header->length=len;
   cur_header->next=0;
   cur_header->prev=prev_header;
   //
   prev_header->next=cur_header;
   
   return (void *)(chunk_start+sizeof(header_t));
}

void kfree(void *p)
{
   header_t *header=(header_t*)((uint32_t)p-sizeof(header_t));//p指向的是有效的内存 
   header->allocated=0;

   glue_chunk(header);
}

void alloc_chunk(uint32_t start,uint32_t len)
{
   while(start+len>heap_max)
   {
      get_page(heap_max,1,kernel_directory);
      heap_max+=0x1000;//liangtodo
   }
}
void free_chunk(header_t *chunk)
{
   //只有最后一个chunk块才能调用这个函数
   if(chunk->prev==0)
   {
      heap_first=0;
   }
   else
   {
      chunk->prev->next=0;
   }
   while((heap_max-(uint32_t)chunk)>=0x1000)
   {
      heap_max-=0x1000;
      free_frame(get_page(heap_max,1,kernel_directory));//这里是heap_max吗 liangdodo
   }
}

void split_chunk(header_t *chunk,uint32_t len)
{
   
   if(chunk->length-len>sizeof(header_t))
   {
      header_t *new_chunk=(header_t *)((uint32_t)chunk+len);//bug helex的bug 下一个chunk是这个chunk+需要的size啊 他直接这个chunk->size

      new_chunk->prev=chunk;
      new_chunk->next=chunk->next;
      new_chunk->allocated=0;
      new_chunk->length=chunk->length-len;

      chunk->next=new_chunk;
      chunk->length=len;
      new_chunk->next->prev=new_chunk;//这个他也没写
   }
}

void glue_chunk(header_t *chunk)
{
   if(chunk->next&&chunk->next->allocated==0)
   {
      chunk->length=chunk->length+chunk->next->length;
      if(chunk->next->next)
      {
         chunk->next->next->prev=chunk;
      }
      chunk->next=chunk->next->next;
   }

   if(chunk->prev&&chunk->prev->allocated==0)
   {
      chunk->prev->length=chunk->length+chunk->prev->length;
      chunk->prev->next=chunk->next;
      if(chunk->next)
      {
         chunk->next->prev=chunk->prev;
      }  
      chunk=chunk->prev;
   
   }

   if(chunk->next==0) free_chunk(chunk);
}