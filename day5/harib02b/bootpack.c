
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
	char cyls,leds,vmode,reserve;
	short scrnx,scrny;
	char *vram;
};

static char font_A[16] = {
0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24,
0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x00, 0x00
};

extern char hankaku[4096];



//function
void io_hlt(void);
void set_palette(int start, int end, unsigned char *rgb);
void init_palette(void);
void boxfill8(unsigned char *vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1);
void sleep1s(void);
void init_screen(unsigned char *vram,int x_size,int y_size);
void putfont8(char *vram,int xsize,int x,int y,char color,char *font);
void HariMain(void)
{
	init_palette();

	struct BOOTINFO *binfo=0x0ff0;//这个是在asmhead中确定的值

	init_screen(binfo->vram,binfo->scrnx,binfo->scrny);
	int xpos=0,i=0;
	for(i=0;i<3;++i,xpos+=8)
		putfont8(binfo->vram,binfo->scrnx,xpos,0,COL8_000000,font_A);
	


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
void sleep1s(void )
{
	int i;
	for(i=0;i<1e7;++i);
}
void init_screen(unsigned char *vram,int xsize,int ysize)
{
	boxfill8(vram, xsize, COL8_008484, 0, 0, xsize - 1, ysize - 29);
	boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize - 29, xsize - 1, ysize - 1);
	//boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize - 28, xsize - 1, ysize - 28);
	//boxfill8(vram, xsize, COL8_FFFFFF, 0, ysize - 27, xsize - 1, ysize - 27);
	// boxfill8(vram, xsize, COL8_FFFFFF, 3, ysize - 24, 59, ysize - 24);
	// boxfill8(vram, xsize, COL8_FFFFFF, 2, ysize - 24, 2, ysize - 4);
	// boxfill8(vram, xsize, COL8_848484, 3, ysize - 4, 59, ysize - 4);
	// boxfill8(vram, xsize, COL8_848484, 59, ysize - 23, 59, ysize - 5);
	// boxfill8(vram, xsize, COL8_000000, 2, ysize - 3, 59, ysize - 3);
	// boxfill8(vram, xsize, COL8_000000, 60, ysize - 24, 60, ysize - 3);
	// boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize - 4, ysize - 24);
	// boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize - 4);
	// boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize - 3, xsize - 4, ysize - 3);
	// boxfill8(vram, xsize, COL8_FFFFFF, xsize - 3, ysize - 24, xsize - 3, ysize - 3);
}
void putfont8(char *vram,int xsize,int x,int y,char color,char *font)
{
	int i;
	char d;//font_data
	for(i=0;i<16;++i)
	{
		d=font[i];
		int j;
		for(j=7;j>=0;--j)
		{
			if(d>>j&1) 
				vram[(y+i)*xsize+x+7-j]=color;
		}
	}
	return ;
}