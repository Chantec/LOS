#include "timer.h"
#include "isr.h"
#include "console.h"
#include "common.h"
#include "sched.h"
#include "printk.h"

uint32_t tick=0;

uint32_t curr_pic;

uint32_t curr_eip;

static void timer_callback(registers_t regs)
{
    // tick++;
    // console_puts("Tick: ");
    // console_put_dec(tick,rc_black,rc_white);
    // console_putc('\n');


    
    curr_eip=regs.eip;
    //printk("timer::eip: %8x\n",curr_eip);
    //printk("timer: eip :%8x",curr_eip);
    
    schedule();
    
}

void init_timer(uint32_t frequency)
{
    //注册函数
    register_interrupt_handler(IRQ0,timer_callback);

    //初始化pic interval timer
    //4个端口
    //0x40-0x42 模式0-3 data 0x43 command   

    uint32_t divisor=1193180/frequency;

    outb(0x43,0x36);

    //先传低位 再传高位
    outb(0x40,divisor&0xff);
    outb(0x40,(divisor>>8)&0xff);
}
