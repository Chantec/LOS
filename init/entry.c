#include "types.h"
#include "console.h"
#include "printk.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"

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
    init_gdt();
    init_idt();

    console_clear(); 
    put_logo();

    //init over


   
    // asm volatile ("int $0x3");//这个似乎是不可屏蔽中断

    asm volatile ("sti");
    init_timer(200);//200hz qemu不能准确模仿时钟

    
    return 0;
}
