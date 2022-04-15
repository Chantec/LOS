#include "types.h"
#include "console.h"
#include "printk.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"

#include "paging.h"


#include "kb.h"

#include "kheap.h"



//liangtodo 这个用的时候得取地址 
extern uint32_t kern_start;
extern uint32_t kern_end;


void puts(char *str)
{
    console_puts_color(str,rc_black,rc_green);
    console_putc('\n');
}
void put_logo()
{
    puts("  _      _                      ____   _____ ");
    puts(" | |    (_)                    / __ \\ / ____|");
    puts(" | |     _  __ _ _ __   __ _  | |  | | (___  ");
    puts(" | |    | |/ _` | '_ \\ / _` | | |  | |\\___ \\ ");
    puts(" | |____| | (_| | | | | (_| | | |__| |____) |");
    puts(" |______|_|\\__,_|_| |_|\\__, |  \\____/|_____/ ");
    puts("                        __/ |                ");
    puts("                       |___/                 ");

}

int kern_entry()
{
    console_clear(); 
    
    init_gdt();
    init_idt();
    init_kb();
    //init_heap();
    init_paging();

    //打印信息
    put_logo();
    puts("Welcome to liang OS!");

    //中断处理
    
    // asm volatile ("int $0x3");//这个似乎是不可屏蔽中断

    asm volatile ("sti");
    //init_timer(200);//200hz qemu不能准确模仿时钟似乎

    


    //内存管理

    printk("kernel in memory start : 0x%X\n",&kern_start);
    printk("kernel in memory end : 0x%X\n",&kern_end);
    //尝试分页错误
    // uint32_t *ptr = (uint32_t*)0xA0000000;
    // uint32_t do_page_fault = *ptr;


    //heap

    init_heap();

    
    void *a=kmalloc(10);
    printk("a 0x%8x\n",a);
    void *b=kmalloc(20);
    printk("b 0x%8x\n",b);
    void *c=kmalloc(30);
    printk("c 0x%8x\n",c);

    kfree(a);
    kfree(b);
    void *d=kmalloc(30);
    printk("d 0x%8x\n",d);
     void *e=kmalloc(30);
    printk("e 0x%8x\n",e);

    
    
    
   
    
    return 0;
}
