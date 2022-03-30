#include "gdt.h"

gdt_entry_t gdt_entris[5];//gdt
gdt_ptr_t   gdt_ptr;//指向gdt首地址

//构造gdt
static void gdt_set_gate(int32_t num,uint32_t base,uint32_t limit,uint8_t access,uint8_t gran);

//初始化gdt
void init_gdt()
{
    //limit 一共是5个
    gdt_ptr.limit=sizeof(gdt_entry_t)*5-1;
    gdt_ptr.base=(uint32_t)&gdt_entris;

    gdt_set_gate(0,0,0,0,0);//intel 要求 第一个描述符全是0
    //access P DPL DT Type   
    //1001 1010
    //1001 0010
    //1111 1010
    //1111 0010

    //DPL  Descriptor privilege level - Ring 0 - 3. 前两个kernel 后两个user
    //type 决定是code segment 还是data segment
    gdt_set_gate(1,0,0xFFFFFFFF,0x9A,0xCF);
    gdt_set_gate(2,0,0xFFFFFFFF,0x92,0xCF);
    gdt_set_gate(3,0,0xFFFFFFFF,0xFA,0xCF);
    gdt_set_gate(4,0,0xFFFFFFFF,0xF2,0xCF);


    gdt_flush((uint32_t)&gdt_ptr);//这个为什么是32位的 现在还不是用户模式
}



static void gdt_set_gate(int32_t num,uint32_t base,uint32_t limit,uint8_t access,uint8_t gran)
{
    gdt_entris[num].base_low=base&0xFFFF;//16bit
    gdt_entris[num].base_mid=(base>>16)&0xFF;//8bit
    gdt_entris[num].base_high=(base>>24)&0xFF;//8bit

    gdt_entris[num].limit_low=limit&0xFFFF;//16bit
    gdt_entris[num].granularity=(limit>>16)&0x0F;//4bit

    gdt_entris[num].access=access;
    gdt_entris[num].granularity|=gran&0xF0;//gran 8bit 低4位是limit 已经在limit中包含了
}