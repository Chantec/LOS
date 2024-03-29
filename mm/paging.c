#include "paging.h"
#include "string.h"
#include "kheap.h"
#include "printk.h"

//指向已用空间的end
extern uint32_t kern_end;
extern uint32_t placement_address;

//表示位图 frames是申请的表示位图 nframe num_frame
uint32_t *frames;
uint32_t nframe;

//给位图置1
static void set_frame(uint32_t frame_addr)
{
    uint32_t frame=frame_addr/0x1000;//bug001 这个之前写的是frame/0x1000
    uint32_t idx=frame/32;
    uint32_t off=frame%32;
    frames[idx]|=(0x1<<off);
}
//给位图置0
static void clear_frame(uint32_t frame_addr)
{
    uint32_t frame=frame/0x1000;
    uint32_t idx=frame/32;
    uint32_t off=frame%32;
    frames[idx]&=~(0x1<<off);
}
//检查位图的值
static uint32_t test_frame(uint32_t frame_addr)
{
    uint32_t frame=frame_addr/0x1000;
    uint32_t idx=frame/32;
    uint32_t off=frame%32;
    return (frames[idx]&(0x1<<off));
}
//找到第一个页框
static uint32_t first_frame()
{
    for(int i=0;i<nframe/32;++i)
    {
        if(frames[i]!=0xFFFFFFFF)
        {
            for(int j=0;j<32;++j)
            {
                uint32_t to_test=0x1<<j;
                if(!(frames[i]&to_test))
                {
                    return i*32+j;
                }
            }
        }
    }
}


//allocate and free
void alloc_frame(page_t *page,int is_kernel,int is_writeable)
{
    if(page->frame!=0)
    {
        printk_color(rc_black,rc_green,"page_not zero page->frame 0x %8x\n",page->frame);
        return ;
    }
    else
    {
        uint32_t idx=first_frame();
        if(idx==(uint32_t)-1)//这个是啥意思
        {
            //PANIC("No free frames!");
        }
        set_frame(idx*0x1000);//我们使用了
        page->present =1;
        page->rw=(is_writeable)?1:0;
        page->user=(is_kernel)?0:1;
        page->frame=idx;
    }
}

//free
void free_frame(page_t *page)
{
    uint32_t frame=page->frame;
    if(frame==0)
    {
        return ;
    }
    else 
    {
        clear_frame(frame);
        page->frame=0x0;
    }
}

void init_paging()
{
    placement_address=&kern_end;//bug 000

    //设置物理内存的属性 这个我可以用multiboot协议 这里按照james的教程假定为16MB
    uint32_t mem_end_page=0x1000000;

    nframe=mem_end_page/0x1000;//有这么几个页
    frames=(uint32_t *)kmalloc1(nframe/32);//分配空间用来放位图

    memset(frames,0,sizeof (frames));//

    //开始制作 page directory

    kernel_directory=(page_directory_t *)kmalloc_a1(sizeof(page_directory_t));
    memset(kernel_directory,0,sizeof (page_directory_t));
    current_directory=kernel_directory;

    //分配一下heap使用的页表
    for(uint32_t i =KHEAP_START;i<KHEAP_START+KHEAP_INITIAL_SIZE;i+=0x1000)
        get_page(i,1,kernel_directory);

    //把内核使用到的地址 做一个 相等映射
    uint32_t i=0;
    
    while(i<placement_address)//liangtodo
    {
        alloc_frame(get_page(i,1,kernel_directory),0,0);
        i+=0x1000;
    }
    
    //bug liangtodo meiyou fenpei  frame so that ...

    for(uint32_t i =KHEAP_START;i<KHEAP_START+KHEAP_INITIAL_SIZE;i+=0x1000)
        alloc_frame(get_page(i,1,kernel_directory),1,1);


    register_interrupt_handler(14,&page_fault);

    //
    switch_page_directory(kernel_directory);

    //cr0 分页开启
    uint32_t cr0;
    asm volatile("mov %%cr0,%0":"=r"(cr0));
    
    cr0|=0x80000000;//enable paging
    
    asm volatile("mov %0,%%cr0"::"r"(cr0));
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory=dir;
    //cr3->page_dir
    asm volatile("mov %0,%%cr3"::"r"(&(dir->tablesPhysical)));
}

//make=1 如果不存在就创建他
page_t *get_page(uint32_t address,int make,page_directory_t *dir)
{
    
    address/=0x1000;

    uint32_t table_idx =address/1024;//得到这个page在table中的索引

    if(dir->tables[table_idx])
    {
        return &(dir->tables[table_idx]->pages[address%1024]);
    }
    else if(make)
    {
        uint32_t tmp;
        dir->tables[table_idx]=(page_table_t *)kmalloc_ap1(sizeof(page_table_t),&tmp);//tmp physical memory
        memset(dir->tables[table_idx],0,0x1000);
        dir->tablesPhysical[table_idx]=tmp|0x7;//111 usermode write present
        return &(dir->tables[table_idx]->pages[address%1024]);
    }
    else 
    {
        return 0;
    }
}

void page_fault(registers_t regs)
{
    uint32_t faulting_addr;
    //cr2->faulting_addr
    asm volatile ("mov %%cr2,%0":"=r"(faulting_addr));
    
    uint32_t err_code=regs.err_code;
    int present=!(err_code&0x1);
    int rw=err_code&0x2;
    int us=err_code&0x4;
    int reserved =err_code&0x8;
    int id=err_code&0x10;

    // Bit 0
    //     If set, the fault was not because the page wasn't present. If unset, the page wasn't present.
    // Bit 1
    //     If set, the operation that caused the fault was a write, else it was a read.
    // Bit 2
    //     If set, the processor was running in user-mode when it was interrupted. Else, it was running in kernel-mode.
    // Bit 3
    //     If set, the fault was caused by reserved bits being overwritten.
    // Bit 4
    //     If set, the fault occurred during an instruction fetch.

    console_puts("Page fault!( ");
    if (present) {console_puts("present ");}
    if (rw) {console_puts("read-only ");}
    if (us) {console_puts("user-mode ");}
    if (reserved) {console_puts("reserved ");}
    console_puts(") at ");
    console_put_hex(faulting_addr,rc_black,rc_white);
    console_puts("\n");
    //PANIC("Page fault");

    while(1) ;
}
