#ifndef INCLUDE_PRINTK_H_
#define INCLUDE_PRINTK_H_

#include "types.h"
extern uint32_t read_eip();

typedef __builtin_va_list va_list;

#define va_start(ap, last)         (__builtin_va_start(ap, last))
#define va_arg(ap, type)           (__builtin_va_arg(ap, type))
#define va_end(ap) 


#include "console.h"

void printk(const char *format, ...); 
void printk_color(real_color_t back,real_color_t fore,const char *format, ...);

#endif