#include "isr.h"
#include "console.h"

isr_t interrupt_handler[256];//中断处理程序


void isr_handler(registers_t regs)
{
    console_puts_color("recieved interrupt: ",rc_black,rc_green);
    console_put_dec(regs.int_no,rc_black,rc_green);
    console_putc('\n');

    if(interrupt_handler[regs.int_no]!=0)
    {
        isr_t handler=interrupt_handler[regs.int_no];
        handler(regs);
    }
}

void irq_handler(registers_t regs)
{
    if(regs.int_no>=40)//来自slave 
    {
        outb(0xA0,0x20);//发送reset signal
    }
    outb(0x20,0x20);

    if(interrupt_handler[regs.int_no]!=0)
    {
        isr_t handler=interrupt_handler[regs.int_no];
        handler(regs);
    }
}


void register_interrupt_handler(uint8_t n,isr_t handler)
{
    interrupt_handler[n]=handler;
}