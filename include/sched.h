#ifndef INCLUDE_SCHED_H_
#define INCLUDE_SCHED_H_

#include "task.h"
//就绪
extern PCB_t *ready_procs;
//阻塞 
extern PCB_t *block_procs;
//现在正在运行的
extern PCB_t *current;
//调度函数
void schedule();

//初始化
void init_sched();

//切换准备
void change_task_to(PCB_t *next);

//切换 汇编实现
extern void switch_to(struct context *prev,struct context *next);

#endif