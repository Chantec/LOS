#ifndef INCLUDE_PRINTK_H_
#define INCLUDE_PRINTK_H_

void printk(const char *format, ...); 
void printk_color(real_color_t back,real_color_t fore,const char *format, ...);

#endif