#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include "types.h"

//写byte
void outb(uint16_t port,uint8_t value);
//读byte
uint8_t inb(uint16_t port);
//读word
uint16_t inw(uint16_t port);


#endif