#include <stdio.h> //这个库不是我自己写的

#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

//
struct BOOTINFO
{
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};
//test font
static char font_A[16] = {
	0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24,
	0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x00, 0x00};

extern char hankaku[4096]; //一共是256*16 每个字符有16项 一共是256个字符

//GDT IDT
struct SEGMENT_DESCRIPTOR
{
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR
{
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

//function
void io_hlt(void);
void set_palette(int start, int end, unsigned char *rgb);
void init_palette(void);
void boxfill8(unsigned char *vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1);
void sleep1s(void);
void init_screen(unsigned char *vram, int x_size, int y_size);
void putfont8(char *vram, int xsize, int x, int y, char color, char *font);
void putfonts8(char *vram, int xsize, int x, int y, char color, char *str);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);


void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

void HariMain(void)
{
	init_palette();

	struct BOOTINFO *binfo = 0x0ff0; //这个是在asmhead中确定的值

	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

	putfonts8(binfo->vram, binfo->scrnx, 0, 0, 0, "welcome to LOS");
	putfonts8(binfo->vram, binfo->scrnx, 0, 20, 0, "this is day5's code");

	char str[40];
	sprintf(str, "boot info screen x is %d", binfo->scrnx);
	putfonts8(binfo->vram, binfo->scrnx, 0, 40, 0, str);

	char mouse[16][16];
	init_mouse_cursor8(mouse, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, (binfo->scrnx) / 2, (binfo->scrny) / 2, mouse, 16);


	//初始化 gdt idt
	init_gdtidt();


fin:
	io_hlt();
	goto fin;
}
void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00, /* 0:黑 */
		0xff, 0x00, 0x00, /* 1:亮红 */
		0x00, 0xff, 0x00, /* 2:亮绿 */
		0xff, 0xff, 0x00, /* 3:亮黄 */
		0x00, 0x00, 0xff, /* 4:亮蓝 */
		0xff, 0x00, 0xff, /* 5:亮紫 */
		0x00, 0xff, 0xff, /* 6:浅亮蓝 */
		0xff, 0xff, 0xff, /* 7:白 */
		0xc6, 0xc6, 0xc6, /* 8:亮灰 */
		0x84, 0x00, 0x00, /* 9:暗红 */
		0x00, 0x84, 0x00, /* 10:暗绿 */
		0x84, 0x84, 0x00, /* 11:暗黄 */
		0x00, 0x00, 0x84, /* 12:暗青 */
		0x84, 0x00, 0x84, /* 13:暗紫 */
		0x00, 0x84, 0x84, /* 14:浅暗蓝 */
		0x84, 0x84, 0x84 /* 15:暗灰 */};
	set_palette(0, 15, table_rgb);
	return;
	/* C语言中的static char语句只能用于数据，相当于汇编中的DB指令 */
}
void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags(); /* 记录中断许可标志的值*/
	io_cli();				   /* 将中断许可标志置为0，禁止中断 */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++)
	{
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags); /* 复原中断许可标志 */
	return;
}
void boxfill8(unsigned char *vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; ++y)
		for (x = x0; x <= x1; ++x)
			vram[y * xsize + x] = color;
}
void sleep1s(void)
{
	int i;
	for (i = 0; i < 1e7; ++i)
		;
}
void init_screen(unsigned char *vram, int xsize, int ysize)
{
	boxfill8(vram, xsize, COL8_008484, 0, 0, xsize - 1, ysize - 29);
	boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize - 29, xsize - 1, ysize - 1);
}
void putfont8(char *vram, int xsize, int x, int y, char color, char *font)
{
	int i;
	char d; //font_data
	for (i = 0; i < 16; ++i)
	{
		d = font[i];
		int j;
		for (j = 7; j >= 0; --j)
		{
			if (d >> j & 1)
				vram[(y + i) * xsize + x + 7 - j] = color;
		}
	}
	return;
}
void putfonts8(char *vram, int xsize, int x, int y, char color, char *str)
{
	while (str[0] != '\0')
	{
		putfont8(vram, xsize, x, y, color, hankaku + str[0] * 16);
		x += 8;
		str++;
	}
}
void init_mouse_cursor8(char *mouse, char bc)
/* 准备鼠标指针（16×16） */
{
	static char cursor[16][16] = {"**************..",
								  "*OOOOOOOOOOO*...",
								  "*OOOOOOOOOO*....",
								  "*OOOOOOOOO*.....",
								  "*OOOOOOOO*......",
								  "*OOOOOOO*.......",
								  "*OOOOOOO*.......",
								  "*OOOOOOOO*......",
								  "*OOOO**OOO*.....",
								  "*OOO*..*OOO*....",
								  "*OO*....*OOO*...",
								  "*O*......*OOO*..",
								  "**........*OOO*.",
								  "*..........*OOO*",
								  "............*OO*",
								  ".............***"};
	int x, y;
	for (y = 0; y < 16; y++)
	{
		for (x = 0; x < 16; x++)
		{
			if (cursor[y][x] == '*')
			{
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == 'O')
			{
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == '.')
			{
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}
//vxsize:vram xsize 屏幕的x
//pxsize:picture xsize
//pysize
//px0 py0 想要显示的相对位置
//char buf picture
//bxsize buf 的xsize
void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++)
	{
		for (x = 0; x < pxsize; x++)
		{
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}
void init_gdtidt(void)
{
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)0x00270000;//这个位置存放gdt
	struct GATE_DESCRIPTOR *idt = (struct GATE_DESCRIPTOR *)0x0026f800;//这个位置存放idt
	int i;
	/* GDT的初始化 */
	for (i = 0; i < 8192; i++)
	{
		set_segmdesc(gdt + i, 0, 0, 0);
	}
	set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
	set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
	load_gdtr(0xffff, 0x00270000);
	/* IDT的初始化 */
	for (i = 0; i < 256; i++)
	{
		set_gatedesc(idt + i, 0, 0, 0);
	}
	load_idtr(0x7ff, 0x0026f800);
	return;
}
//这俩函数真看不懂
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
	if (limit > 0xfffff)
	{
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low = limit & 0xffff;
	sd->base_low = base & 0xffff;
	sd->base_mid = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high = (base >> 24) & 0xff;
	return;
}
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
	gd->offset_low = offset & 0xffff;
	gd->selector = selector;
	gd->dw_count = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high = (offset >> 16) & 0xffff;
	return;
}