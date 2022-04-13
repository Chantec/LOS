#ifndef INCLUDE_PAGING_H_
#define INCLUDE_KHEAP_H_

#include "types.h"
#include "isr.h"

//页
typedef struct page
{
    uint32_t present : 1;
    uint32_t rw : 1;       // set if writable
    uint32_t user : 1;     // set if user mode
    uint32_t accessed : 1; //
    uint32_t dirty : 1;
    uint32_t unused : 7;
    uint32_t frame : 20;
} page_t;

//一级页表 table
typedef struct page_table
{
    page_t pages[1024];
}page_table_t;

//二级页表 directory
typedef struct page_directory
{
    page_table_t *tables[1024];//指向1024个页表

    uint32_t tablesPhysical[1024];//这个只是记住他们的地址 用于给cr3 用于访问 liangtodo

    uint32_t physicalAddr;//这个目前还没用到 还不知道什么意思
}page_directory_t;

void init_paging();

void switch_page_directory(page_directory_t *dir);

page_t *get_page(uint32_t addr,int make,page_directory_t *dir);

void page_fault(registers_t regs);


//kernel's page dir
page_directory_t *kernel_directory;
//current ~
page_directory_t *current_directory;


void alloc_frame(page_t *page,int is_kernel,int is_writeable);

void free_frame(page_t *page);



#endif