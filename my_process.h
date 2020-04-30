#ifndef _PROCESS_H_
#define _PROCESS_H_
#include <sys/types.h>
#define child_CPU 1
#define parent_CPU 0

#define _unit_(){volatile unsigned long i;for(i = 0;i<1000000UL;i++);}

struct process{
    char name[32];
    pid_t pid;
    int ready;
    int exec;
};

int process_assign_CPU(int pid, int core);
int process_execute(struct process proc);
int process_block(int pid);
int process_wakeup(int pid);

#endif