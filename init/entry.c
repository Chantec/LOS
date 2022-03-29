#include "types.h"
#include "console.h"
#include "printk.h"
#include "gdt.h"


//test
void puts(char *str)
{
    console_puts(str);
    console_putc('\n');
}
int kern_entry()
{
    init_gdt();
    
    console_clear();
    puts("hello this is my OS!");   
    // printk("%d\n%c\n%s\n%x\n",1,'L',"hello world!",12);
    // printk_color(rc_black,rc_green,"%d\n%c\n%s\n%x\n",1,'L',"hello world!",12);
    

    return 0;
}
