#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include "types.h"
#include "paging.h"

#define STACK_SIZE 4096

//进程状态
typedef 
enum task_state
{
    TASK_UNINIT=0,
    TASK_SLEEPING=1,
    TASK_RUNNABLE=2,
    TASK_ZOMBIE=3
}task_state;
//上下文
struct context
{
    uint32_t esp,ebp,ebx,esi,edi,dflags,eip;
};
//内存相关
struct mm_struct
{
    page_directory_t *dir;
};

typedef int32_t pid_t;
extern pid_t now_pid;

//PCB
typedef 
struct task_struct 
{
    task_state state;
    pid_t pid;
    void * stack;//暂时不实现了
    struct mm_struct *mm;//内核态也不用管
    struct context context;
    struct task_struct* next;
}PCB_t;





//内核线程的创建和结束
int32_t kernel_thread(int(*fn)(void *));
void kthread_exit();

#endif