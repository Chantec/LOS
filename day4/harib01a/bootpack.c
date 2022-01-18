
void io_hlt(void);


void HariMain(void)
{
	int i;
	for(i=0xa0000;i<=0xaffff;++i)
	{
		char *p=i;
		*p=i&0x0f;
	}

fin:
	io_hlt();
	goto fin;

}
