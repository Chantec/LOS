#include "string.h"
#include "types.h"
//below has not been tested yet beacasue my gcc(in windows ) does not work! I am sorry!
void memcpy(uint8_t *dest,const uint8_t *src,uint32_t len)
{
    for(int i=0;i<len;++i)
        dest[i]=src[i];
}
//liangtodo 这里之前是void *
void memset(uint8_t *dest,uint8_t val,uint32_t len)
{
    //这里为什么不直接写成 uint8_t * dest liangtodo
    // uint8_t *dst=(uint8_t*) dest;
    // for(int i=0;i<len;++i)
    //     dst[i]=val;

    uint8_t *temp=(uint8_t*) dest;
    for(;len!=0;len--) *temp++=val;
}
//liangtodo 这也改了
void bzero(uint8_t *dest,uint32_t len)
{
    memset(dest,0,len);
}
int strcmp(const char *str1,const char *str2)
{
    while(*str1&&*str2&&*str1==*str2)
    {
        str1++,str2++;
    }
    return str1-str2;
}

char *strcpy(char *dest,const char *src)
{
    char *backup=dest;
    while(*src)
    {
        *dest=*src;
        dest++,src++;
    }
    *dest='\0';
    return backup;
}

char *strcat(char *dest,const char *src)
{
    char *backup=dest;
    while(*dest)
    {
        dest++;
    }
    while(*src)
    {
        *dest=*src;
        src++,dest++;
    }
    *dest='\0';
    return backup;
}

int strlen(const char *str)
{
    int len=0;
    while(*str)
    {
        len++;
        str++;
    }
    return len;
}
