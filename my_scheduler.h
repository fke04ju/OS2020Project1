#ifndef _SCHEDULING_H_
#define _SCHEDULING_H_
#include "my_process.h"

#define FIFO 1
#define RR 2
#define SJF 3
#define PSJF 4

int scheduling(struct process *proc, int proc_num, int name);

#endif