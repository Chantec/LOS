//超级简化版的 printk
//只支持我经常使用的 %d %c %s %x
//不支持宽度精度等等 只提供了最最基本的支持
//等有时间再来补坑
#include "printk.h"
#include "string.h"

static void vsprintf(char *buff, const char *format, va_list args);

void printk_color(real_color_t back,real_color_t fore,const char *format, ...)
{
    // 避免频繁创建临时变量
    static char buff[1024];
    va_list args;

    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);

    console_puts_color(buff,back,fore);
}
void printk(const char *format, ...)
{
    // 避免频繁创建临时变量
    static char buff[1024];
    va_list args;

    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);

    console_puts(buff);
}
 #define do_div(n,base) ({ \
		int __res; \
		__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
		__res; })


static char *number(char *str, int num, int base,int sign,int size)//添加了int sign int size  size==-1 表示不用补齐
{   int bool_size=(size==-1)?0:1;
    char  tmp[36];
    const char *digits = "0123456789ABCDEF";
    int i; // tmp的指针

    if (sign && num < 0)
    {
        num = -num;
        *str++ ='-';
    }

    i = 0;

    //这里有一个bug 就是 num可能会被识别成负数 虽然我们设置的变量是sign=0 我们这里的解决方法是使用Linux里的to_div
   
    // do
    // {
    //     tmp[i++] = digits[num%base];
    //     num/=base;
    //     size--;
    // } while (num);

    if(num==0)
    {
        tmp[i++]='0';
        size--;
    }
    else 
    {
        while(num!=0)
        {
            tmp[i++]=digits[do_div(num,base)];
            size--;
        }
    }

    if(bool_size)
    {
        while(size-->0) *str++='0';
    }


    while (i-- > 0)
    {
        *str++ = tmp[i];
    }
    return str;
}

static void vsprintf(char *buff, const char *format, va_list args)
{
    int len;
    int i;
    char *str;
    char *s;//%s

    for (str = buff; *format; ++format)
    {
        if (*format != '%')
        {
            *str++ = *format;
            continue;
        }

        format++;//skip %

        int size=-1;

        if(*format<='9' && *format>='0')//如果要求占位
        {
            size=*format-'0';
            format++;
        }

        switch (*format)
        {
        case 'c':

            *str++ = (unsigned char)va_arg(args, int);//liangtodo why int why unsigned
            break;

        case 's':
            s = va_arg(args, char *);
            len = strlen(s);
            for (i = 0; i < len; ++i)
            {
                *str++ = *s++;
            }
            break;

        case 'd':
            str = number(str, va_arg(args, unsigned long), 10,1,size);//liangtodo why long
            break;
        case 'x':
        case 'X':
            str = number(str, va_arg(args, unsigned long), 16,0,size);
            break;
        }
    }
    *str = '\0';
}