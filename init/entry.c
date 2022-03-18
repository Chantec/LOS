#include "types.h"
#include "console.h"


//test
void puts(char *str)
{
    console_puts(str);
    console_putc('\n');
}
int kern_entry()
{
    console_clear();
    puts("hello this is my OS! Here we run the print test:");
    console_put_dec(100,rc_black,rc_green);
    puts("");
    console_put_dec(0,rc_black,rc_green);
    puts("");
    console_put_dec(1234,rc_black,rc_green);
    puts("");
    console_put_hex(21,rc_black,rc_green);

    return 0;
}
