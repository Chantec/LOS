#include "types.h"
#include "console.h"

int kern_entry()
{

    console_clear();
    console_puts_color("hello,world!",rc_black,rc_green);
   

    return 0;
}
