#include "sched.h"
#include "task.h"
#include "kheap.h"
#include "printk.h"
#include "string.h"

//就绪
PCB_t *ready_procs;
//阻塞 
PCB_t *block_procs;
//现在正在运行的
PCB_t *current;

pid_t now_pid=0;

extern char kern_stack[STACK_SIZE];

//初始化
void init_sched()
{
    current=(PCB_t*)kmalloc(sizeof(PCB_t));
    memset(current,0,sizeof(current));


    current->state=TASK_RUNNABLE;
    current->pid=now_pid++;
    current->stack=kern_stack;
    current->mm=NULL;
    current->next=NULL;


    ready_procs=current;
    //asm volatile("sti");
}


//调度函数
void schedule()
{
    if(current)
        change_task_to(current->next);
}

extern uint32_t curr_eip;

//切换准备  
void change_task_to(PCB_t *next)
{
    if(next!=NULL)
    {
        printk("cur next %d %d\n",current->pid,next->pid);
        PCB_t* prev=current;
        current=next;
        
        ready_procs=current;//把prev拿出来 
        PCB_t *ptr=ready_procs;
        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
        }
        ptr->next=prev;//把prev放到队列末尾
        prev->next=NULL;//bug 找了半天

        switch_to(&(prev->context),&(current->context));
        //仅切换ip


        //save the eip
      
        //prev->context.eip=curr_eip;

        //restore eip
        //asm("mov %0, %%ecx;\n\t sti;\n\t jmp *%%ecx"::"r"(current->context.eip));

        //asm("mov %0,%%ecx;\n\t sti;\n\t push %%ecx\n\t ret"::"r"(current->context.eip));
    }
}

