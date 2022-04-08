#include "isr.h"
#include "console.h"
#include "common.h"
#include "printk.h"


int bool_shift;

char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		


char shift_kbdus[128]={
		/* first row - indices 0 to 14 */
		0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
		/* second row - indices 15 to 28 */
		'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
		/* 29 = Control, 30 - 41: third row */
		0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
		/* fourth row, indices 42 to 54, zeroes are shift-keys*/
		0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*',
		/* Special keys */
		0,							  // ALT - 56
		' ',						  // Space - 57
		0,							  // Caps lock - 58
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 to F10 - 59 to 68
		0,							  // Num lock - 69
		0,							  // Scroll lock - 70
		0,							  // Home - 71
		0,							  // Up arrow - 72
		0,							  // Page up - 73
		'-',
		0, // Left arrow - 75
		0,
		0, // Right arrow -77
		'+',
		0, // End - 79
		0, // Dowm arrow - 80
		0, // Page down - 81
		0, // Insert - 82
		0, // Delete - 83
		0, 0, 0,
		0, // F11 - 87
		0, // F12 - 88
		0, // All others undefined
	};
static void kb_getchar(registers_t regs)
{
    uint8_t scancode = inb(0x60);
    if(scancode==42||scancode==54)//l shirt r shift
    {
        bool_shift=1;
    }
    if((scancode&0x80)==0)//bug 
    {
      char ch;
      if(bool_shift==0)
        ch = kbdus[scancode];
      else 
        ch=shift_kbdus[scancode];
      console_putc_color(ch, rc_black, rc_green);
    }
    else 
    {
        if((scancode&0x7f)==42||(scancode&0x7f)==54)
        {
            bool_shift=0;
        }
    }
}

void init_kb(uint32_t frequency)
{
    //注册函数
    register_interrupt_handler(IRQ1, kb_getchar);
}
