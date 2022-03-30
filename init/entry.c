#include "types.h"
#include "console.h"
#include "printk.h"
#include "gdt.h"
#include "idt.h"

int kern_entry()
{
    init_gdt();
    init_idt();
    
    console_clear(); 

    printk("Welcome to my os!Here we are testing INT\n");

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    

    return 0;
}
