#include "task.h"
#include "kheap.h"
#include "string.h"
#include "sched.h"
#include "printk.h"
int32_t kernel_thread(int(*fn)(void *))
{
    PCB_t *new_task=(PCB_t*)kmalloc(sizeof(PCB_t));
    memset(new_task,0,sizeof(new_task));
    uint32_t new_stack=(uint32_t)kmalloc(STACK_SIZE);


    new_task->state=TASK_RUNNABLE;
    new_task->pid=now_pid++;
    new_task->stack=(void *)new_stack;
    new_task->mm=NULL;
    new_task->next=NULL;

    
    uint32_t *stack_top = (uint32_t *)((uint32_t)new_stack + STACK_SIZE);
	*(--stack_top) = (uint32_t)fn;

    new_task->context.esp = (uint32_t)new_task + STACK_SIZE - sizeof(uint32_t);

    
   // new_task->context.eip=fn;//加了个取地址 就不对了 地址<内核开始地址

    //printk("task_id %d fn:0x%8x\n",new_task->pid,new_task->context.eip);

    //将其插入队尾
    new_task->context.dflags=0x200;
    PCB_t*ptr=ready_procs;
    while(ptr->next!=NULL)
        ptr=ptr->next;
    ptr->next=new_task;

    return new_task->pid;
}