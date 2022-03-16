#include "console.h"
#include "common.h"

//VGA begin addr
static uint16_t *video_memory=(uint16_t*)0xB8000;

//cursor 
static uint8_t cursor_x=0;
static uint8_t cursor_y=0;


void move_cursor()
{
    //80*25 x y
    uint16_t cursor_location=cursor_x+cursor_y*80;
    //14 15 cursor_location hign low
    outb(0x3D4,14);
    outb(0x3D5,cursor_location>>8);
    outb(0x34,15);
    outb(0x3D5,cursor_location);
}

void console_clear()
{
    //generate black space
    //black  white 
    //space 0x20
    uint8_t attribute_byte=(0<<4)|(15&0x0f);
    uint16_t blank=(attribute_byte<<8)|0x20;
    int i;
    for(i =0;i<80*25;i++)
    {
        video_memory[i]=blank;
    }
    cursor_x=0,cursor_y=0;
    move_cursor();

}

void console_putc_color(char ch,real_color_t back,real_color_t fore)
{
    uint8_t attribute_byte=(back<<4)|(fore&0x0f);
    uint16_t c_=(attribute_byte<<8)|ch;//to discriminate the ch and c, I use c_ even if it does't good

    //0x08 backspace 
    if(ch==0x08&&cursor_x)
    {
        cursor_x--;
    }
    else if(ch==0x09)//0x09 tab
    {
        cursor_x=(cursor_x+8)&~(8-1);//do not understand liangtodo
    }
    else if(ch=='\r')
    {
        cursor_x=0;
    }
    else if(ch=='\n')
    {
        cursor_x=0;
        cursor_y++;
    }
    else if(ch>=' ')
    {
        video_memory[cursor_x+cursor_y*80]=c_;
        cursor_x++;
    }

    if(cursor_x>=80)
    {
        cursor_x=0;
        cursor_y++;
    }

    if(cursor_y>=25)
    {
        scroll();
    }

    move_cursor();
}

void console_putc(char ch)
{
    //defalt color 0 15
    console_putc_color(ch,0,15);
}
void console_puts(char *str)
{
    while(*str)
    {
        console_putc(*(str++));
    }
}
void console_puts_color(char *str,real_color_t back,real_color_t fore)
{
    while(*str)
    {
        console_putc_color(*(str++),back,fore);
    }
}

void scroll()
{
    uint8_t attribute_byte=(0<<4)|(15&0x0f);
    uint16_t blank=(attribute_byte<<8)|0x20;

    if(cursor_y>=25)
    {
        for(int i=0*80;i<24*80;i++)
        {
            video_memory[i]=video_memory[i+80];
        }

        for(int i=24*80;i<25*80;i++)
        {
            video_memory[i]=blank;
        }

        cursor_y=24;
    }
}
