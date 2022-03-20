//超级简化版的 printk
//只支持我经常使用的 %d %c %s %x
//不支持宽度精度等等 只提供了最最基本的支持
//等有时间再来补坑
#include "console.h"
#include "vargs.h"

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
static char *number(char *str, int num, int base)
{
    int size=8;//只是当16进制时用到
    char sign, tmp[36];
    const char *digits = "0123456789ABCDEF";
    int i; // tmp的指针

    if (num < 0)
    {
        sign = '-';
        num = -num;
    }
    else
    {
        sign = 0;
    }

    i = 0;

    do
    {
        tmp[i++] = digits[(num % base)];
        num /= base;
        size--;
    } while (num);

    if (sign)
    {
        *str++ = sign;
    }

    if (base == 8)
    {
        *str++ = '0';
    }
    else if (base == 16)
    {
        *str++ = '0';
        *str++ = 'X';
    }

    if(base==16)
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
            str = number(str, va_arg(args, unsigned long), 10);//liangtodo why long
            break;
        case 'x':
            str = number(str, va_arg(args, unsigned long), 16);
            break;
        }
    }
    *str = '\0';
}
