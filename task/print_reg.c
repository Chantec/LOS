#include "printk.h"
#include "types.h"
void print_reg(uint32_t reg)
{
    printk("print_reg: %8x\n",reg);
}