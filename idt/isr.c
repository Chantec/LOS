#include "isr.h"
#include "console.h"

void isr_handler(registers_t regs)
{
    console_puts_color("recieved interrupt: ",rc_black,rc_green);
    console_put_dec(regs.int_no,rc_black,rc_green);
    console_putc('\n');
}