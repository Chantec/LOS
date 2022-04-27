#include "types.h"
#include "console.h"
#include "printk.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "paging.h"
#include "kb.h"
#include "kheap.h"
#include "task.h"
#include "sched.h"

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

char kern_stack[STACK_SIZE];

int flag=0;
int fn()
{
    int cnt=0;
    while(1)
    {
        //if(flag==0)
        {
            printk_color(rc_black,rc_red,"A");
            flag=1;
        }
    }
}


int kern_entry()
{
    console_clear(); 
    
    init_gdt();
    init_idt();
    init_kb();
    //init_heap();
    init_paging();

    // 切换内核栈
	uint32_t kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE);
	asm volatile ("mov %0, %%esp\n\t"
			"xor %%ebp, %%ebp" : : "r" (kern_stack_top));



    //打印信息
    put_logo();
    puts("Welcome to liang OS!");

    //中断处理
    
    // asm volatile ("int $0x3");//这个似乎是不可屏蔽中断

    asm volatile ("sti");
    //init_timer(200);//200hz qemu不能准确模仿时钟似乎

    


    //内存管理

    // printk("kernel in memory start : 0x%X\n",&kern_start);
    // printk("kernel in memory end : 0x%X\n",&kern_end);
    //尝试分页错误
    // uint32_t *ptr = (uint32_t*)0xA0000000;
    // uint32_t do_page_fault = *ptr;


    //heap

    init_heap();

    //堆测试

    // void *a=kmalloc(10);
    // printk("a 0x%8x\n",a);
    // void *b=kmalloc(20);
    // printk("b 0x%8x\n",b);
    // void *c=kmalloc(30);
    // printk("c 0x%8x\n",c);

    // kfree(a);
    // kfree(b);
    // void *d=kmalloc(30);
    // printk("d 0x%8x\n",d);
    //  void *e=kmalloc(30);
    // printk("e 0x%8x\n",e);





    //内核线程测试

    asm("sti");
    init_timer(1);
    //asm("mov %0, %%ecx;\n\t  jmp *%%ecx "::"r"(fn));
  

    
    init_sched();

    kernel_thread(fn);
    //flag=1;
    while(1)
    {
        if(flag==1)
        {
            console_putc_color('B',rc_black,rc_green);
            flag=0;
        }
    }
   
    
    return 0;
}
