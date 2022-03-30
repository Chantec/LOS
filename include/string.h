#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "types.h"
//liangtodo
//memset 为啥使用的void *(h's) 

void memcpy(uint8_t *dest,const uint8_t *src,uint32_t len);

void memset(uint8_t *dest,uint8_t val,uint32_t len);

void bzero(uint8_t *dest,uint32_t len);

int strcmp(const char *str1,const char *str2);

char *strcpy(char *dest,const char *src);

char *strcat(char *dest,const char *src);

int strlen(const char *str);

#endif